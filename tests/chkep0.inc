uz80as=$top_builddir/src/uz80as
asm=$nom.asm
obj=$nom.obj
lst=$nom.lst
eok=$nom.eok
err=$nom.err
$uz80as $asm 2>$err
test ! -f $obj && test ! -f $lst && diff --strip-trailing-cr $err $eok && rm -f $err
