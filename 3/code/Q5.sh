#!/bin/bash

function func1() {
     i=0
    while [ $i -lt 5 ];
    do
        let j=i
        let j=j+1
        while [ $j -gt 0 ];
        do
            
            printf "|"
            if [ $j -eq 1 ];
            then
                printf "_"
            else
                printf " "
            fi
        ((j--))
        done
        echo ""
        ((i++))
    done
}



function func2() {
    i=0
    while [ $i -lt 12 ];
    do
        if [ $i -lt 6 ];
        then
            let j=5-i
            while [ $j -gt 0 ];
            do
            printf " "
            ((j--))
            done
            
            let j=i+1
            while [ $j -gt 0 ];
            do
            printf ". "
            ((j--))
            done
        else

            let j=i-6
            while [ $j -gt 0 ];
            do
            printf " "
            ((j--))
            done

            let j=12-i
            while [ $j -gt 0 ];
            do
            printf ". "
            ((j--))
            done

        fi
    ((i++))
    echo ""
    done

}


function func3() {
    i=0
    while [ $i -lt 5 ];
    do
        let j=i+1;
        let k=i+1;
        while [ $j -gt 0 ];
        do
            printf "$k" 
            ((j--))
        done
    ((i++))
    echo ""
    done

}




echo "Please enter 1 or 2 or 3 :"
read x
echo ""
if [ $x -eq 1 ];
then
    func1
fi

if [ $x -eq 2 ];
then
    func2
fi


if [ $x -eq 3 ];
then
    func3
fi
