#!/bin/sh

# ===========================================================================
# uz80as, an assembler for the Zilog Z80 and several other microprocessors.
#
# Generates the test files.
# ===========================================================================

uz80as=../build/src/uz80as
tasm=../../tasm/tasm

function ugen ()
{
	local name
	name=$1
	echo $name
	shift
	${uz80as} $* $name.asm $name.bok $name.lok 2>$name.eok
}

function tgen ()
{
	echo $1
	${tasm} -b -80 $1.asm $1.bok $1.lokt
	head -n -1 $1.lokt >$1.lok
        rm -f $1.lokt
}

# tgen tasm
# tgen zx48k
tgen main
ugen z80
ugen z80u -u
ugen hd64180 -thd64180
ugen gbcpu -tgbcpu
ugen dp2200 -tdp2200
ugen dp2200ii -tdp2200ii
ugen i4004 -ti4004
ugen i4040 -ti4040
ugen i8008 -ti8008
ugen i8021 -ti8021
ugen i8022 -ti8022
ugen i8041 -ti8041
ugen i8048 -ti8048
ugen i8051 -ti8051
ugen i8080 -ti8080
ugen i8085 -ti8085
ugen i8085u -u -ti8085
ugen mos6502 -tmos6502
ugen r6501 -tr6501
ugen g65sc02 -tg65sc02
ugen r65c02 -tr65c02
ugen r65c29 -tr65c29
ugen w65c02s -tw65c02s
ugen mc6800 -dUZ80AS -tmc6800
ugen mc6801 -dUZ80AS -tmc6801
ugen m68hc11 -dUZ80AS -tm68hc11
ugen tasmfail
ugen longlabel
ugen orgbelow
ugen orgabove
ugen strseq
ugen blockbelow
ugen blockabove
ugen fillneg
ugen fillabove
ugen genabove
ugen fillall
ugen noend
ugen genafterend
ugen multiend
ugen chkbelow
ugen chkabove
ugen chkeq
ugen chk
ugen pcmul
ugen unoff
ugen unoffok -x
ugen div
ugen mod
ugen fillcmdln -fab
ugen unary
