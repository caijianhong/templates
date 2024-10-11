#!/bin/bash
p=`basename $PWD`
make $p ||exit 1
ulimit -s 1000000
for f in `find . -name '*.in'`; do
  \time ./$p < $f | diff - ${f%.*}.out -sqBZ
done


