CC=gcc
CFLAGS=
LIBS = -lpthread
SOURCES=Producer.c Consumer.c Factory.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECS=producer consumer factory

all: $(SOURCES) $(EXECS)
	
producer: Producer.c
	$(CC) $(CFLAGS) -o $@ $? $(LIBS)

consumer: Consumer.c
	$(CC) $(CFLAGS) -o $@ $? $(LIBS)

factory: Factory.c
	$(CC) $(CFLAGS) -o $@ $? $(LIBS)

clean:
	rm -rf *o $(EXECS)

.cpp.o:
	$(CC) $(CFLAGS) -o $@ $? $(LIBS)
