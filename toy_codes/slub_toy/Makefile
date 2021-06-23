#author : kown hyoman
#email : exportd2016@gmail.com

CC=gcc 
CFLAGS += -g -fno-omit-frame-pointer          
LDFLAGS += -rdynamic                       

SOURCES = main.c 


OBJECTS = ${SOURCES:.c=.o} 

target = out 


$(target) : $(OBJECTS)
	$(CC)-o $(target) $(OBJECTS) $(LDFLAGS) ${CFLAGS} 

clean:
	rm $(target) *.o
