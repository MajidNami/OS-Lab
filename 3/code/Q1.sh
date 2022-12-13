#!/bin/bash

echo "Please inter the first number : "
read x
echo "Please inter the second number : "
read y

range="^[0-9]+$"
if ! [[ $x =~ $range ]]  || ! [[ $y =~ $range ]];
then
    echo "Inputs are not valid"
    exit 1
fi


#if [ $# != 2 ];
#then
#   echo "There are more than 2 Inputs"
#fi


((sum = $x + $y))
echo "sum is $sum" 

if [ $x -gt $y ]
then
    echo "$x is maximum"
else
    echo "$y is maximum"
fi


