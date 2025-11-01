bin_PROGRAMS = syndre
bindir = $(prefix)/$(PACKAGE)

MKW = $(PYTHON) $(top_srcdir)/util/mkwrappers

syndre_SOURCES = \
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
	player.c \
	player.h \
	sound.c \
	sound.h \
	syndre.sx \
	timer.c \
	timer.h \
	osunix.c \
	osunix.h \
	oswindws.c \
	oswindws.h \
	util.c \
	util.h

syndre_CPPFLAGS =

syndre_WRAPPERS = wrappers_dos.o wrappers_game.o wrappers_libc.o wrappers_util.o

syndre_RCFLAGS = \
  -I"$(top_srcdir)/src" -I"$(builddir)" \
  $(RCFLAGS)

if HAS_WINDRES
syndre_RESRCS = syndre_stdres.res
else
syndre_RESRCS =
endif

# Pretending to contain c++ source so that Automake select c++ linker
nodist_EXTRA_syndre_SOURCES = dummy.cxx

# Libraries included with the project
syndre_PROJECT_LIBS = ../lib/libbullfrog.a ../lib/libbfsound.a

$(syndre_OBJECTS) $(bin_PROGRAMS): $(syndre_PROJECT_LIBS)

syndre_LDADD = \
  $(syndre_WRAPPERS) $(syndre_RESRCS)

../lib/libbullfrog.a: ../bflibrary/libbullfrog.a
	$(MAKE) -C $(<D) DESTDIR="../" prefix="" install

../lib/libbfsound.a: ../bfsoundlib/libbfsound.a
	$(MAKE) -C $(<D) DESTDIR="../" prefix="" install

$(syndre_WRAPPERS:.o=.sx): %.sx: $(top_srcdir)/conf/%.conf
	$(AM_V_GEN)$(MKW) $(MKWFLAGS) -o $@ $<

.sx.o:
	$(AM_V_CC)$(CPP) $(CPPFLAGS) $< $(ASFILTER) \
		| $(CCAS) -x assembler -c $(CCASFLAGS) -o $@ -

if HAS_WINDRES
%.res: $(top_srcdir)/res/%.rc
	$(WINDRES) $(syndre_RCFLAGS) -i $< -J rc -o $@ -O coff
endif

# Include dynamic libraries in the package
if TARGET_WINDOWS
install-exec-hook:
	mkdir -p "$(DESTDIR)${prefix}/$(PACKAGE)"
	## Find main executable dependencies
	$(eval lib_SHARED_INSTALL := $(shell objdump -p syndre$(EXEEXT) | \
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
