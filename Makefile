CFLAGS=-g -Wall 
OBJECTS=z80.o memory.o mmu.o main.o

all: $(OBJECTS)
	cc -o gbemulator $(OBJECTS) $(CFLAGS) $(LDFLAGS) 

%.o: %.c
	cc -c -o $@ $< $(CFLAGS) $(LDFLAGS)

install: all 
	install -d $(DESTDIR)/$(PREFIX)/bin/ 
	install devpkg $(DESTDIR)/$(PREFIX)/bin/ 

clean:
	rm -f *.o
	rm -f gbemulator
	rm -rf *.dSYM

