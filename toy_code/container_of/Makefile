CC=gcc

SOURCES = main.c


OBJECTS = ${SOURCES:.c=.o}

target = out 


$(target) : $(OBJECTS)
	$(CC) -o $(target) $(OBJECTS) 

clean:
	rm $(target) *.o
