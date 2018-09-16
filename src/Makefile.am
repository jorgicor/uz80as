# ===========================================================================
# uz80as, an assembler for the Zilog Z80 and several other microprocessors.
# ===========================================================================

AM_CFLAGS = $(WARN_CFLAGS)
bin_PROGRAMS = uz80as

# When using per object flags, automake generates the object files with
# uz80as_ prefixed, just in case we build the same program with different
# flags. But we don't need it for this project; AM_CFLAGS is enough.
# uz80as_CFLAGS = $(AM_CFLAGS)

uz80as_SOURCES = \
	ngetopt.c ngetopt.h \
	main.c \
	options.c options.h \
	utils.c utils.h \
	err.c err.h \
	incl.c incl.h \
	sym.c sym.h \
	expr.c expr.h \
	exprint.c exprint.h \
	pp.c pp.h \
	list.c list.h \
	uz80as.c uz80as.h \
	targets.c targets.h \
	target/z80.c \
	target/gbcpu.c \
	target/dp2200.c \
	target/i4004.c \
	target/i8008.c \
	target/i8048.c \
	target/i8080.c \
	target/mos6502.c
