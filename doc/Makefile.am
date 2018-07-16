# ===========================================================================
# uz80as, an assembler for the Zilog Z80 and several other microprocessors.
# ===========================================================================
#
AM_MAKEINFOHTMLFLAGS = --no-headers --no-split
info_TEXINFOS = uz80as.texi
dist_noinst_DATA = z80tb.texi hd64180tb.texi gbcputb.texi i8080tb.texi \
		   i8085tb.texi
dist_man_MANS = uz80as.1

uz80as.1: $(top_srcdir)/src/main.c $(top_srcdir)/configure.ac
	$(HELP2MAN) --name="Micro Z80 Assembler" --version-string="$(PACKAGE_STRING)" --output=$@ $(top_builddir)/src/uz80as$(EXEEXT)
