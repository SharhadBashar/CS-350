#!/bin/bash
LOOP=100
echo Running Lock Test $LOOP times
for i in `seq 1 $LOOP`;
do
    sys161 kernel "sy2;q"
done
for i in `seq 1 $LOOP`;
do
    sys161 kernel "uw1;q"
done
echo Finished Lock tests