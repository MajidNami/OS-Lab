
echo "please enter operand (+,-,*,/) :"
read op
echo "please enter the first number :"
read x
echo "please enter the second number :"
read y

case $op in
    "+")
        ((result = $x + $y))
     ;;   
    "-")
        ((result = $x - $y))
    ;;
    "*")
        ((result = $x * $y))
    ;;
    "/")
        if [ $y -eq 0 ]
        then 
            echo "Divide error by 0"
            exit 1
        fi
        ((result = $x / $y))
    ;;
    *)
        echo "Invalid operand"
        exit 1
    esac
    echo "result is $result" 