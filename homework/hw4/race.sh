#!/bin/bash

FILE=./race.out

if ! test -f "$FILE"; then
    echo 1 >> $FILE
fi
for i in `seq 1 100`
do
    LASTNUM=$(tail -n 1 $FILE)
    LASTNUM=$((LASTNUM + 1))
    echo $LASTNUM >> $FILE
done
