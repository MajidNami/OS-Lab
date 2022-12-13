#!/bin/bash

while :
do 
    echo "please enter your number :"
    read n
    let sum=0
    let new=0
    while [ $n -gt 0 ]
    do
        let r=n%10
        let new=new*10
        let new=new+r
        let sum=sum+r
        let n=n/10
    done
    echo "sum of digits = $sum"
    echo "new number is : $new"
done
