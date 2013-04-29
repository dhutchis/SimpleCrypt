
for i in {0..25} 
do 
    ./crypt.bin -seed $i -shift `cat inputfile`
done