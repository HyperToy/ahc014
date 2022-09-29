for i in `seq -f '%04g' 0 999`
do  
    echo $i
    ./a < tools/in/$i.txt > tools/out/$i.txt
done