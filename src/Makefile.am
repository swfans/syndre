bin_PROGRAMS = bfgame

EXTRA_DIST = Makefile.windows windows.config.h

MKW = $(PYTHON) $(top_srcdir)/util/mkwrappers

bfgame_SOURCES = \
	bflib_basics.c \
	bflib_dernc.c \
	bflib_fileio.c \
	bflib_filelst.c \
	bflib_memory.c \
	bflib_picture.c \
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
	bfgame.S \
	timer.c \
	timer.h \
	unix.c \
	unix.h \
	util.c \
	util.h \
	windows.c \
	windows.h

bfgame_LDADD = wrappers_dos.o wrappers_game.o wrappers_libc.o wrappers_util.o

$(bfgame_LDADD:.o=.S): %.S: $(top_srcdir)/conf/%.conf
	$(AM_V_GEN)$(MKW) $(MKWFLAGS) -o $@ $<

.S.o:
	$(AM_V_CC)$(CPP) $(CPPFLAGS) $< $(ASFILTER) \
		| $(CCAS) -x assembler -c $(CCASFLAGS) -o $@ -

CLEANFILES = wrappers_*.S
DISTCLEANFILES = *~
