asm="$srcdir"/$nom.asm
bok="$srcdir"/$nom.bok
lok="$srcdir"/$nom.lok
obj="$builddir"/$nom.obj
lst="$builddir"/$nom.lst
ltp="$builddir"/$nom.ltp

$uz80as $asm

tr -d '\r' <$lst >$ltp
mv -f $ltp $lst
rm -f $ltp

cmp $obj $bok && diff $lst $lok && rm -f $obj $lst
