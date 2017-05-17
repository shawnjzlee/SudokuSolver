for ((i = 1; i <= $2; i++))
do
	echo -e "\nTesting" $1
	for ((j = 0; j < 15; j++))
	do
		time=$(./sudoku_solver -s 9 -f $1 -t $i | grep -o '[0-9]\.[0-9]*')
		echo $time
# 		sum=$(python -c "print $sum+$time")
	done
    # echo $sum
done