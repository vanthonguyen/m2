# !/bin/sh
for i in {1..50}
do
    ./matching ../images/$i"r.jpg" ../images/$i"l.jpg" 
done
