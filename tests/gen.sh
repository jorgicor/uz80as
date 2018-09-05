#!/bin/sh

# ===========================================================================
# uz80as, an assembler for the Zilog Z80 and several other microprocessors.
#
# Generates the test files.
# ===========================================================================

uz80as=../src/uz80as
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
# tgen Spanish128_ROM0
tgen main
ugen z80
ugen z80u -u
ugen hd64180 -thd64180
ugen gbcpu -tgbcpu
ugen dp2200 -tdp2200
ugen dp2200ii -tdp2200ii
ugen i8008 -ti8008
ugen i8080 -ti8080
ugen i8085 -ti8085
ugen i8085u -u -ti8085
ugen mos6502 -tmos6502
ugen r6501 -tr6501
ugen g65sc02 -tg65sc02
ugen r65c02 -tr65c02
ugen r65c29 -tr65c29
ugen w65c02s -tw65c02s
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
