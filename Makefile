OUT=dabble
CFILES=main.c dabble.c gfx.c
LUAFILES=dabble.lua test.lua

OUTDIR=out
SRCDIR=src
OBJDIR=obj
LUADIR=lua

CFLAGS+=-Wall -g -fPIC -D DBL_LUA_PATH="\"./$(LUADIR)/?.lua\"" `pkg-config --cflags sdl lua5.2`
LDFLAGS+=`pkg-config --libs sdl lua5.2`

SOURCES=$(addprefix $(SRCDIR)/, $(CFILES))
OBJECTS=$(addprefix $(OBJDIR)/, $(CFILES:.c=.o))
LUAOUTS=$(addprefix $(OUTDIR)/$(LUADIR)/, $(LUAFILES))

all: $(OUTDIR)/$(OUT) $(LUAOUTS)

$(OUTDIR)/$(LUADIR)/%.lua: $(SRCDIR)/$(LUADIR)/%.lua
	mkdir -p $(OUTDIR)/$(LUADIR)
	cp $< $@

$(OUTDIR)/$(OUT): $(OBJECTS)
	mkdir -p $(OUTDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c objdir
	$(CC) $(CFLAGS) -c -o $@ $<

objdir:
	mkdir -p $(OBJDIR)

clean:
	rm -r $(OBJDIR) $(OUTDIR)

run: all
	cd $(OUTDIR); ./$(OUT)