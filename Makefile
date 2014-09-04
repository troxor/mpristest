mpris:
	gcc `pkg-config --libs --cflags gio-2.0` mpris.c -o m

all: mpris

