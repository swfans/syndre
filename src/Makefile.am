bin_PROGRAMS = bfgame
bindir = $(prefix)/$(PACKAGE)

MKW = $(PYTHON) $(top_srcdir)/util/mkwrappers

bfgame_SOURCES = \
	../bfsoundlib/include/ail.h \
	../bfsoundlib/src/ail.c \
	../bfsoundlib/include/ailss.h \
	../bfsoundlib/src/ailss.c \
	../bfsoundlib/include/aila.h \
	../bfsoundlib/src/aila.c \
	../bfsoundlib/include/aildebug.h \
	../bfsoundlib/src/aildebug.c \
	../bfsoundlib/include/mssdig.h \
	../bfsoundlib/src/mssdig.c \
	../bfsoundlib/include/mssxmidi.h \
	../bfsoundlib/src/mssxmidi.c \
	../bfsoundlib/include/bfsvaribl.h \
	../bfsoundlib/src/variable.c \
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
	osunix.c \
	osunix.h \
	oswindws.c \
	oswindws.h \
	util.c \
	util.h

bfgame_CPPFLAGS = \
  -I"$(top_srcdir)/bflibrary/include" \
  -I"$(top_srcdir)/bfsoundlib/include"

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

# Libraries included with the project
bfgame_PROJECT_LIBS = ../lib/libbullfrog.a

$(bfgame_OBJECTS) $(bin_PROGRAMS): $(bfgame_PROJECT_LIBS)

bfgame_LDADD = \
  $(bfgame_WRAPPERS) $(bfgame_RESRCS)

../lib/libbullfrog.a: ../bflibrary/libbullfrog.a
	$(MAKE) -C $(<D) DESTDIR="$(top_builddir)" prefix="" install

$(bfgame_WRAPPERS:.o=.sx): %.sx: $(top_srcdir)/conf/%.conf
	$(AM_V_GEN)$(MKW) $(MKWFLAGS) -o $@ $<

.sx.o:
	$(AM_V_CC)$(CPP) $(CPPFLAGS) $< $(ASFILTER) \
		| $(CCAS) -x assembler -c $(CCASFLAGS) -o $@ -

if HAS_WINDRES
%.res: $(top_srcdir)/res/%.rc
	$(WINDRES) $(bfgame_RCFLAGS) -i $< -J rc -o $@ -O coff
endif

# Include dynamic libraries in the package
if TARGET_WINDOWS
install-exec-hook:
	mkdir -p "$(DESTDIR)${prefix}/$(PACKAGE)"
	## Find main executable dependencies
	$(eval lib_SHARED_INSTALL := $(shell objdump -p bfgame$(EXEEXT) | \
		sed -n 's/\s*\(DLL Name:\|NEEDED\)\s*\(.*\)$$/\2/p' | \
		xargs -I {} find $(shell dirname $(shell which ${CXX})) -name '{}'))
	## Find sub-dependencies for the previous dependencies
	$(eval lib_SHARED_INSTALL += $(shell echo -n '$(lib_SHARED_INSTALL)' | \
        xargs -d ' ' -I {} objdump -p '{}' | \
		sed -n 's/\s*\(DLL Name:\|NEEDED\)\s*\(.*\)$$/\2/p' | \
		xargs -I {} find $(shell dirname $(shell which ${CXX})) -name '{}'))
	## Remove duplicates
	$(eval lib_SHARED_INSTALL := $(sort $(lib_SHARED_INSTALL)))
	cp $(lib_SHARED_INSTALL) $(DESTDIR)${prefix}/$(PACKAGE)
endif

CLEANFILES = wrappers_*.sx
DISTCLEANFILES = *~
