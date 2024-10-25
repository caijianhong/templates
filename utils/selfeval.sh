#!/bin/bash
p=`basename $PWD`
make $p ||exit 1
ulimit -v 1000000 #1G
for f in `find . -name '*.in'`; do
  \time ./$p < $f | diff - ${f%.*}.out -sqBZ
done


