#!/bin/sh

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

function check {
	echo "**** Checking $1"
	"$@"
	local status=$?
	if [ $status -ne 0 ]; then
		echo -e "${RED}ERROR${NC} with $1"
	else
		echo -e "${GREEN}OK${NC}"
	fi
	return $status
}

top_builddir=..
export top_builddir

tests='zx48k'

for t in $tests; do
	check ./${t}.test
done
