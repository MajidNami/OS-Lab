#!/bin/bash

echo "Please enter file name :"
read filename
echo "Please enter start line :"
read x
echo "Please enter finish line : "
read y

head -n $y $filename | tail -n $(($y - $x + 1))