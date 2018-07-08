# ===========================================================================
# uz80as, a macro assembler for Z80 based microprocessors.
# ===========================================================================
#
AM_MAKEINFOHTMLFLAGS = --no-headers --no-split
info_TEXINFOS = uz80as.texi
dist_man_MANS = uz80as.1

uz80as.1: $(top_srcdir)/src/main.c $(top_srcdir)/configure.ac
	$(HELP2MAN) --name="Micro Z80 Assembler" --version-string="$(PACKAGE_STRING)" --output=$@ $(top_builddir)/src/uz80as$(EXEEXT)
