#!/bin/bash
p=`basename $PWD`
make $p || exit 1
ulimit -s 1048576
ulimit -v 1048576
ulimit -t 6
for f in `find . -name '*.in'`; do
  \time ./$p < $f | diff - ${f%.*}.ans -sqBZ
done
