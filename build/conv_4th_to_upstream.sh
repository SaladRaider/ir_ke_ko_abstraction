#!/bin/bash


for i in $(seq -f "_%03g.csv" 0 $1)
do
    cut -d',' -f11 distributions/$i > distributions_4th_upstream/$i
    echo "converted distributions/$i to distributions_4th_upstream/$i"
done