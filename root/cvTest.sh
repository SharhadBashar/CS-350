#!/bin/bash
LOOP=100
echo Running Conditional Variables Test $LOOP times
for i in `seq 1 $LOOP`;
do
    sys161 kernel "sy3;q"
done
echo Finished Conditional Variables tests