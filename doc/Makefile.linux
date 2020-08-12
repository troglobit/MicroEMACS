CFLAGS=		-O2 -g -Dlinux -Wno-implicit-function-declaration -Wno-implicit-int

OFILES=		ansi.o basic.o bind.o buffer.o display.o file.o \
		fileio.o hp150.o line.o lock.o main.o random.o region.o \
		search.o spawn.o tcap.o termio.o vt52.o window.o word.o

CFILES=		ansi.c basic.c bind.c buffer.c display.c  file.c \
		fileio.c hp150.c line.c lock.c main.c random.c region.c \
		search.c spawn.c tcap.c termio.c vt52.c window.c word.c

HFILES=		estruct.h edef.h efunc.h epath.h

all:		emacs

emacs:		$(OFILES)
		$(CC) $(CFLAGS) $(OFILES) -lc -o emacs

clean:
		$(RM) $(OFILES)

distclean:	clean
		$(RM) emacs *~

$(OFILES):	$(HFILES)
