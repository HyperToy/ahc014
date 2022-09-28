for i in `seq -f '%04g' 0 999`
do 
    ./a < tools/in/$i.txt > tools/out/$i.txt
done