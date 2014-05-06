# !/bin/sh
f=cgamma
echo > $f 
for c in 300 
do
    for gamma in 0.05 0.1 0.5 1.0 2.0
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
