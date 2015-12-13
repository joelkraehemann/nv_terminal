CFLAGS = `pkg-config --cflags gtk+-2.0 vte`
LDFLAGS = `pkg-config --libs gtk+-2.0 vte` -lutil

all: nv_emulator nv_terminal
	echo "compile all"

nv_emulator: nv_emulator.o nv_terminal_emulator.o
	gcc -o nv_emulator nv_emulator.o nv_terminal_emulator.o $(LDFLAGS) -lm

nv_emulator.o: nv_emulator.c
	gcc -c nv_emulator.c -o nv_emulator.o $(CFLAGS) $(LDFLAGS)

nv_terminal_emulator.o: nv_terminal_emulator.c
	gcc -c nv_terminal_emulator.c -o nv_terminal_emulator.o $(CFLAGS) $(LDFLAGS)

nv_terminal: nv_terminal.o nv_terminal_window.o nv_terminal_menubar.o
	gcc -o nv_terminal nv_terminal.o nv_terminal_window.o nv_terminal_menubar.o $(LDFLAGS) -lm

nv_terminal.o: nv_terminal.c nv_terminal_window.c
	gcc -c nv_terminal.c -o nv_terminal.o $(CFLAGS) $(LDFLAGS)

nv_terminal_window.o: nv_terminal_window.c
	gcc -c nv_terminal_window.c -o nv_terminal_window.o $(CFLAGS) $(LDFLAGS)

nv_terminal_menubar.o: nv_terminal_menubar.c
	gcc -c nv_terminal_menubar.c -o nv_terminal_menubar.o $(CFLAGS) $(LDFLAGS)
