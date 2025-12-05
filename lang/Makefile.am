TEXTDAT = $(PYTHON) $(top_srcdir)/util/textdat.py

langall_eu_ETBLS = \
    $(top_srcdir)/conf/char_encoding_tbl_eu.txt

langalleng_DATFILES_C = \
    eng/guitext.dat

langalleng_POFILES_C = \
    guitext_eng.pot

langallengdir = $(prefix)/$(PACKAGE)/language/eng
langalleng_DATA = $(langalleng_DATFILES_C)

$(langalleng_DATFILES_C): $(langalleng_POFILES_C)
	$(MKDIR_P) "$(@D)"
	$(TEXTDAT) -t "$(word 1,$(langall_eu_ETBLS))" -d $@ -c $+

langallfre_DATFILES_C = \
    fre/guitext.dat

langallfre_POFILES_C = \
    guitext_fre.po

langallfredir = $(prefix)/$(PACKAGE)/language/fre
langallfre_DATA = $(langallfre_DATFILES_C)

$(langallfre_DATFILES_C): $(langallfre_POFILES_C)
	$(MKDIR_P) "$(@D)"
	$(TEXTDAT) -t "$(word 1,$(langall_eu_ETBLS))" -d $@ -c $+

langallita_DATFILES_C = \
    ita/guitext.dat

langallita_POFILES_C = \
    guitext_ita.po

langallitadir = $(prefix)/$(PACKAGE)/language/ita
langallita_DATA = $(langallita_DATFILES_C)

$(langallita_DATFILES_C): $(langallita_POFILES_C)
	$(MKDIR_P) "$(@D)"
	$(TEXTDAT) -t "$(word 1,$(langall_eu_ETBLS))" -d $@ -c $+

EXTRA_DIST = \
	langallfre \
	langallita \
	langalleng
