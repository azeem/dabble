OUT=dabble
SRCDIR=src
OBJDIR=obj
CFILES=main.c dabble.c
CFLAGS+=-Wall -fPIC -D_GNU_SOURCE ${INCLUDE} `pkg-config --cflags lua sdl`
LDFLAGS+=`pkg-config --libs lua sdl`

SOURCES=$(addprefix $(SRCDIR)/, $(CFILES))
OBJECTS=$(addprefix $(OBJDIR)/, $(CFILES:.c=.o))

all: $(OUT)

$(OUT): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm $(OBJECTS) $(OUT)