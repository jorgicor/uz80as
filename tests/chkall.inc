asm="$srcdir"/$nom.asm
eok="$srcdir"/$nom.eok
bok="$srcdir"/$nom.bok
lok="$srcdir"/$nom.lok
obj="$builddir"/$nom.obj
lst="$builddir"/$nom.lst
err="$builddir"/$nom.err
ltp="$builddir"/$nom.ltp
etp="$builddir"/$nom.etp

$uz80as $asm 2>$etp

# Remove \r and remove leading path in error strings.

s_esc="$(echo "$srcdir"/ | sed 's/[]\\.$*{}|+?()[^-]/\\&/g')"
tr -d '\r' <$etp | sed 's|^'$s_esc'||' >$err
rm -f $etp

tr -d '\r' <$lst >$ltp
mv -f $ltp $lst
rm -f $ltp

cmp $obj $bok && diff $lst $lok && diff $eok $err && rm -f $obj $lst $err
