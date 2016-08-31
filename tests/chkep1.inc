uz80as=$top_builddir/src/uz80as
asm=$nom.asm
bok=$nom.bok
obj=$nom.obj
lok=$nom.lok
lst=$nom.lst
eok=$nom.eok
err=$nom.err
$uz80as $asm 2>$err
test ! -f $obj && diff --strip-trailing-cr $err $eok && diff --strip-trailing-cr $lst $lok && rm -f $err $lst
