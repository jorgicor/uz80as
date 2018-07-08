asm="$srcdir"/$nom.asm
eok="$srcdir"/$nom.eok
obj="$builddir"/$nom.obj
lst="$builddir"/$nom.lst
err="$builddir"/$nom.err
etp="$builddir"/$nom.etp

$uz80as $asm 2>$etp

# Remove \r from $etp and remove leading path in error strings.

s_esc="$(echo "$srcdir"/ | sed 's/[]\\.$*{}|+?()[^-]/\\&/g')"
tr -d '\r' <$etp | sed 's|^'$s_esc'||' >$err
rm -f $etp

test ! -f $obj && test ! -f $lst && diff $err $eok && rm -f $err
