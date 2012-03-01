CC=cc
CFLAGS=-g -Wall -Wfatal-errors -std=c99
OBJECTS=z80.o memory.o mmu.o gpu.o main.o

all: $(OBJECTS)
	$(CC) -o gbemulator $(OBJECTS) $(CFLAGS) $(LDFLAGS) 

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

install: all 
	install -d $(DESTDIR)/$(PREFIX)/bin/ 
	install devpkg $(DESTDIR)/$(PREFIX)/bin/ 

clean:
	rm -f *.o
	rm -f gbemulator
	rm -rf *.dSYM

