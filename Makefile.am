ACLOCAL_AMFLAGS = --install -I m4

SUBDIRS = bflibrary conf src util

# Do not install static libraries.
install installdirs: SUBDIRS = conf src util

doc_DATA = AUTHORS COPYING GPL3
docdir = $(prefix)/$(PACKAGE)

res_DATA = res/bfgame_icon.ico res/bfgame_icon.png
resdir = $(prefix)/$(PACKAGE)

instscript_DATA = util/install
instscriptdir = $(prefix)/$(PACKAGE)

EXTRA_DIST = doc res instscript
