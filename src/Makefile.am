bin_PROGRAMS = bfgame

MKW = $(PYTHON) $(top_srcdir)/util/mkwrappers

bfgame_SOURCES = \
	bflib_render.c \
	bflib_render.h \
	display.c \
	display.h \
	dos.c \
	dos.h \
	game_data.c \
	game_data.h \
	game.c \
	game.h \
	keyboard.c \
	keyboard.h \
	main.c \
	mouse.c \
	mouse.h \
	oggvorbis.c \
	oggvorbis.h \
	sound.c \
	sound.h \
	sound_util.c \
	sound_util.h \
	bfgame.sx \
	timer.c \
	timer.h \
	unix.c \
	unix.h \
	util.c \
	util.h \
	windows.c \
	windows.h

bfgame_CPPFLAGS = \
  -I"$(top_srcdir)/bflibrary/include"

bfgame_WRAPPERS = wrappers_dos.o wrappers_game.o wrappers_libc.o wrappers_util.o

bfgame_RCFLAGS = \
  -I"$(top_srcdir)/src" -I"$(builddir)" \
  $(RCFLAGS)

if HAS_WINDRES
bfgame_RESRCS = bfgame_stdres.res
else
bfgame_RESRCS =
endif

# Pretending to contain c++ source so that Automake select c++ linker
nodist_EXTRA_bfgame_SOURCES = dummy.cxx

bfgame_LDADD = \
  $(bfgame_WRAPPERS) $(bfgame_RESRCS) \
  -L$(builddir)/../bflibrary -lbullfrog

$(bfgame_WRAPPERS:.o=.sx): %.sx: $(top_srcdir)/conf/%.conf
	$(AM_V_GEN)$(MKW) $(MKWFLAGS) -o $@ $<

.sx.o:
	$(AM_V_CC)$(CPP) $(CPPFLAGS) $< $(ASFILTER) \
		| $(CCAS) -x assembler -c $(CCASFLAGS) -o $@ -

if HAS_WINDRES
%.res: $(top_srcdir)/res/%.rc
	$(WINDRES) $(bfgame_RCFLAGS) -i $< -J rc -o $@ -O coff
endif

CLEANFILES = wrappers_*.sx
DISTCLEANFILES = *~
