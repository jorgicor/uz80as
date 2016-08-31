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

tgen Spanish128_ROM0

