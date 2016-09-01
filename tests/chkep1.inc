asm="$srcdir"/$nom.asm
bok="$srcdir"/$nom.bok
lok="$srcdir"/$nom.lok
eok="$srcdir"/$nom.eok
obj="$builddir"/$nom.obj
lst="$builddir"/$nom.lst
err="$builddir"/$nom.err
ltp="$builddir"/$nom.ltp
etp="$builddir"/$nom.etp

$uz80as $asm 2>$etp

# Remove \r and remove leading path in error strings.

tr -d '\r' <$etp | sed 's|'$srcdir'/||' >$err
rm -f $etp

tr -d '\r' <$lst >$ltp
mv -f $ltp $lst
rm -f $ltp

test ! -f $obj && diff $err $eok && diff $lst $lok && rm -f $err $lst
