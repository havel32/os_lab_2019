#!/bin/bash

sum=0
count=0
while read arg
do
echo "ghgh"
sum=$(($sum + $arg))
count=$(($count + 1))
done
avg_a=$(bc<<<"scale=3;$sum/$count")
echo "Count: $count"
echo "Average: $D"