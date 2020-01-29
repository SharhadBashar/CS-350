#!/bin/bash
LOOP=100
echo Running Conditional Variables Test $LOOP times
for i in `seq 1 10`;
do
    sys161 kernel "sy3;q"
done