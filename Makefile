OUT=dabble

CC?=gcc
CFLAGS+=-Wall -fPIC -D_GNU_SOURCE ${INCLUDE} `pkg-config --cflags lua5.2`
LDFLAGS+=`pkg-config --libs lua5.2`

#CFILES=main.c

#SOURCES=$(addprefix src/, $(CFILES))
#OBJECTS=$(addprefix obj/, $(CFILES:.c=.o))

srcdir=src/
SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)

all: $(OUT)

$(OUT): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

o.c:
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm $(OBJECTS) $(OUT)
