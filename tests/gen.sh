#!/bin/sh

uz80as=../src/uz80as
tasm=../../tasm/tasm

function ugen ()
{
	echo $1
	${uz80as} $2 $1.asm $1.bok $1.lok 2>$1.eok
}

function tgen ()
{
	echo $1
	${tasm} -b -80 $1.asm $1.bok $1.lokt
	head -n -1 $1.lokt >$1.lok
        rm -f $1.lokt
}

tgen tasm
# tgen Spanish128_ROM0
tgen main
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
ugen gbz80 -cgbz80
