#!/bin/bash


echo "please enter operand (+,-,*,/) :"
read op
echo "please enter the first number :"
read x
echo "please enter the second number :"
read y

case $op in
    "+")
        printf "result is : "
        echo "$x + $y" | bc
     ;;   
    "-")
        printf "result is : "
        echo "$x - $y" | bc
    ;;
    "*")
        printf "result is : "
        echo "$x * $y" | bc
    ;;
    "/")
        if [ $y -eq 0 ]
        then 
            echo "Divide error by 0"
            exit 1
        fi
        printf "result is : "
        echo "$x / $y" | bc
    ;;
    *)
        echo "Invalid operand"
        exit 1
    esac
    