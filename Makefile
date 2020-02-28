all: onirim

onirim: onirim.o

onirim.o: onirim.c

clean:
	$(RM) *.o *.exe
	