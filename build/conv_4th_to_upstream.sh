#!/bin/bash


for i in $(seq -f "_%03g.csv" 0 $1)
do
    cut -d',' -f11 distributions/$i > distributions_4th_upstream/$i
    tail -n +2 distributions_4th_upstream/$i > distributions_4th_upstream/_$i
    echo "converted distributions/$i to distributions_4th_upstream/$i"
done

cat distributions_4th_upstream/__*.csv > distributions_4th_upstream/all.csv
rm distributions_4th_upstream/_*.csv
