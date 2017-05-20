for ((i = 1; i <= $2; i++))
do
	echo -e "\nTesting" $1 "with" $2 "threads\n"
	for ((j = 0; j < 15; j++))
	do
		./sudoku_solver -s 16 -f $1 -t $i
		echo "Iteration $j complete"
	done
done