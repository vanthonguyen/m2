# !/bin/sh
f=gammac
###echo > $f 
for c in 10 100 200 300 400 500
do
    for gamma in 0.5
    do
        echo "-------------$c-$gamma-------------">> $f 
        for i in {0..2}
        do
            echo ./training learning_3_$i vocabulary.xml svm.xml $c $gamma >> $f
            ./training learning_3_$i vocabulary.xml svm.xml $c $gamma >> $f
            ./classifier test_3_$i >> $f
        done
    done
done
