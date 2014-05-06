# !/bin/bash

for i in {1..30}
do
    cp $1/obj"$i"__* $2
done
