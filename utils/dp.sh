#!/bin/bash
make bf activity||exit 1
for i in {1..124123}; do
  echo Testcase $i is running...
  ./dt.py > 0.in
  ./bf <0.in >0.ans
  ./activity <0.in >0.out
  diff 0.out 0.ans||exit 1
done
