g++ the_grandmaster_idea.cpp -o the_grandmaster_idea -O3 -Wall -ltbb -std=c++17
rm *.tgi tmp_single simulation

if [ ! -f total ]
then
	echo 0 > total
fi

for (( i=1; i<=$1; i++ ))
do
	echo "$i) playing..."
	./the_grandmaster_idea

	echo "merging..."
	cat *.tgi >> simulation
	rm *.tgi
done

echo "updating total..."
echo $(( $(wc -l simulation | cut -d' ' -f1) + $(<total) )) > total

echo "merging simulation..."
cat simulation >> single
rm simulation

echo "sorting..."
sort single | uniq > tmp_single
mv tmp_single single

echo "DONE ALL! $(wc -l single) over $(<total)"
