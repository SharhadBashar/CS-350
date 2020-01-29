#!/bin/bash
LOOP=100
echo Running Lock Test $LOOP times
for i in `seq 1 10`;
do
    sys161 kernel "sy2;q"
done
