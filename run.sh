for i in `seq -f '%04g' 0 49`
do  
    echo $i
    ./a < tools/in/$i.txt > tools/out/$i.txt
done