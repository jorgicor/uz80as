# ===========================================================================
# uz80as, an assembler for the Zilog Z80 and several other microprocessors.
# ===========================================================================
#
AM_MAKEINFOHTMLFLAGS = --no-headers --no-split
info_TEXINFOS = uz80as.texi
uz80as_TEXINFOS = z80tb.texi hd64180tb.texi gbcputb.texi \
		  dp2200tb.texi dp2200iitb.texi \
		  i4004tb.texi i4040tb.texi i8008tb.texi \
		  i8021tb.texi i8048tb.texi \
		  i8080tb.texi i8085tb.texi \
		  mos6502tb.texi r6501tb.texi g65sc02tb.texi \
		  r65c29tb.texi w65c02stb.texi
dist_man_MANS = uz80as.1

uz80as.1: $(top_srcdir)/src/main.c $(top_srcdir)/configure.ac
	$(HELP2MAN) --name="Micro Z80 Assembler" --version-string="$(PACKAGE_STRING)" --output=$@ $(top_builddir)/src/uz80as$(EXEEXT)
