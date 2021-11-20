#nohup $(for (( k=0; k<10; k++ )); do ./command.sh; done) &
rm out;
for (( k=0; k<10; k++ )); do
	for (( i=0; i<10000; i+=200 )); do
		rm -rf test_matches_*;
		a=$(( 100*i ));
		echo "$a" start;
		./chess -2 "$a" -3 | grep 'games appear exactly once on' >> out;
		echo "$a" end;
	done
done
