OUT=dabble
CFILES=main.c dabble.c canvas.c script.c trans.c common.c
LUAFILES=test.lua

OUTDIR=out
SRCDIR=src
OBJDIR=obj
LUADIR=lua

CFLAGS+=-Wall -g -fPIC -D DBL_LUA_PATH="\"./$(LUADIR)/?.lua\"" -I/usr/include/lua/5.2 `pkg-config --cflags sdl SDL_gfx`
LDFLAGS+=`pkg-config --libs sdl SDL_gfx` -llua5.2

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

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -r $(OBJDIR) $(OUTDIR)

run: all
	cd $(OUTDIR); ./$(OUT)