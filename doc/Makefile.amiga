CC =		cc
CFLAGS =	-O

OBJ1 =		ansi.o basic.o bind.o buffer.o display.o file.o
OBJ2 =		fileio.o hp150.o line.o lock.o main.o random.o region.o
OBJ3 =		search.o spawn.o tcap.o termio.o vt52.o window.o word.o
OBJ =		obj1.o obj2.o obj3.o

emacs :		$(OBJ)
		$(CC) $(LDFLAGS) -o emacs $(OBJ)

obj1.o :	$(OBJ1)
		join $(OBJ1) as obj1.o

obj2.o :	$(OBJ2)
		join $(OBJ2) as obj2.o

obj3.o :	$(OBJ3)
		join $(OBJ3) as obj3.o

ansi.o :	ansi.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) ansi.c

basic.o :	basic.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) basic.c

bind.o :	bind.c estruct.h edef.h efunc.h epath.h
		$(CC) -c $(CFLAGS) bind.c

buffer.o :	buffer.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) buffer.c

display.o :	display.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) display.c

file.o :	file.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) file.c

fileio.o :	fileio.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) fileio.c

hp150.o :	hp150.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) hp150.c

line.o :	line.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) line.c

lock.o :	lock.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) lock.c

main.o :	main.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) main.c

random.o :	random.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) random.c

region.o :	region.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) region.c

search.o :	search.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) search.c

spawn.o :	spawn.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) spawn.c

tcap.o :	tcap.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) tcap.c

termio.o :	termio.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) termio.c

vt52.o :	vt52.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) vt52.c

window.o :	window.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) window.c

word.o :	word.c estruct.h edef.h efunc.h
		$(CC) -c $(CFLAGS) word.c
