uz80as=$top_builddir/src/uz80as
asm=$nom.asm
obj=$nom.obj
lst=$nom.lst
bok=$nom.bok
lok=$nom.lok
$uz80as $asm
cmp $obj $bok && diff --strip-trailing-cr $lst $lok && rm -f $obj $lst
