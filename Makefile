OUT=dabble
CFILES=main.c dabble.c
LUAFILES=dabble.lua test.lua

OUTDIR=out
SRCDIR=src
OBJDIR=obj
LUADIR=lua

CFLAGS+=-Wall -fPIC -D DBL_LUA_PATH="\"./$(LUADIR)/?.lua\"" `pkg-config --cflags sdl` -I/usr/include/lua/5.2
LDFLAGS+=`pkg-config --libs sdl` -lm -llua5.2

SOURCES=$(addprefix $(SRCDIR)/, $(CFILES))
OBJECTS=$(addprefix $(OBJDIR)/, $(CFILES:.c=.o))
LUAOUTS=$(addprefix $(OUTDIR)/$(LUADIR)/, $(LUAFILES))

all: $(OUTDIR)/$(OUT) $(LUAOUTS)

$(OUTDIR)/$(LUADIR)/%.lua: $(SRCDIR)/$(LUADIR)/%.lua
	mkdir -p $(OUTDIR)/$(LUADIR)
	cp $< $@

$(OUTDIR)/$(OUT): $(OBJECTS)
	mkdir -p $(OUTDIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c objdir
	$(CC) $(CFLAGS) -c -o $@ $<

objdir:
	mkdir -p $(OBJDIR)

clean:
	rm -r $(OBJDIR) $(OUTDIR)