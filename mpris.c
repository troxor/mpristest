#include <gio/gio.h>
#include <stdio.h>

static gchar *status = "";
GDBusMessage *message         = NULL;

const gchar  *path         = "/org/mpris/MediaPlayer2";
const gchar  *intf         = "org.mpris.MediaPlayer2.Player";
const gchar  *meth         = "PlayPause";

static void name_appeared_handler (GDBusConnection *connection,
                                   const gchar *name,
                                   const gchar *name_owner,
                                   gpointer user_data)
{
        g_print ("\n:: %s appeared, bitches\n\n", name);
        message = g_dbus_message_new_method_call ( name, path, intf, meth );
        printf("%s", g_dbus_message_print(message, 3));

        // Create the proxy
        GDBusProxyFlags flags = G_DBUS_PROXY_FLAGS_NONE;
        GDBusInterfaceInfo *info = NULL;
        GDBusProxy *proxy = g_dbus_proxy_new_sync ( connection,
                                                    flags,
                                                    info,
                                                    name, // param from main
                                                    path, // global
                                                    intf, // global
                                                    NULL, // cancellable
                                                    NULL);
        GVariant *vmetadata = g_dbus_proxy_get_cached_property( proxy,
                                                               "Metadata");
        GVariant *vplaystat = g_dbus_proxy_get_cached_property( proxy,
                                                               "PlaybackStatus");
        status = g_strdup( g_variant_get_string( vplaystat, NULL ) );
        g_print ("\n\n::%s\n\n::", status);
        g_print ("%s", g_variant_print( vmetadata, TRUE ) );
}

static void name_vanished_handler (GDBusConnection *connection,
                                   const gchar *name,
                                   gpointer user_data)
{
        status = "not running\n";
        g_print ("\n:: %s", status);
        g_object_unref(message);
}

int main (void) {

        const gchar  *name         = "org.mpris.MediaPlayer2.audacious";
        // Watch well-known name (org...audacious) 
        guint watcher_id = g_bus_watch_name( G_BUS_TYPE_SESSION,
                                  name,
                                  G_BUS_NAME_WATCHER_FLAGS_NONE,
                                  name_appeared_handler,
                                  name_vanished_handler,
                                  NULL,
                                  NULL );
        g_main_loop_run ( g_main_loop_new(NULL, FALSE));
        g_bus_unwatch_name (watcher_id );
        return 0;
}
// vim:  ts=8 sts=8 sw=8 et
