CC=gcc
BINDIR = $(DESTDIR)/usr/bin

raplstat: raplstat.o
	$(CC) -o raplstat raplstat.o

install:
	install --mode=755 raplstat $(BINDIR)/

uninstall:
	rm $(BINDIR)/raplstat

.PHONY: clean
clean:
	rm -f raplstat.o raplstat

