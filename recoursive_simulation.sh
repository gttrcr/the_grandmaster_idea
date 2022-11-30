if [ ! -f counter ]
then
    echo 0 > counter
fi

for (( i=1; i<=$1; i++ ))
do
        #echo "$i) playing..."
        ./the_grandmaster_idea > /dev/null

        #echo "merging..."
        cat *.tgi > simulation

        #echo "counting..."
        echo $(( $(wc -l simulation | cut -d' ' -f1) + $(<counter) )) > counter
        rm *.tgi

        #echo "sorting..."
        sort simulation | uniq >> single
        rm simulation
        sort single | uniq > tmp_single
        mv tmp_single single

        echo "$(wc -l single | cut -d' ' -f1) uniq over $(<counter) simulations"
done

echo "DONE ALL"