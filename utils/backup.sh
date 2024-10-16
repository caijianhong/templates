#!/bin/bash
dst=`dirname $0`/tmp/`date +%H-%M-%S`
mkdir -p $dst
for p in snow camphor card; do
  src=`dirname $0`/$p/$p.cpp
  if [ -e $src ]; then
    cp $src $dst
    if g++ $src -o /dev/null -std=c++14 -lm -O2; then
      echo Problem $p: ok
    else
      echo Problem $p: CE
    fi
  else
    echo Problem $p: not found
  fi
done

