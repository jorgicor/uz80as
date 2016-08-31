uz80as=$top_builddir/src/uz80as
asm=$nom.asm
obj=$nom.obj
lst=$nom.lst
eok=$nom.eok
err=$nom.err
bok=$nom.bok
lok=$nom.lok
$uz80as $asm 2>$err
cmp $obj $bok && diff --strip-trailing-cr $lst $lok && diff --strip-trailing-cr $eok $err && rm -f $obj $lst $err
