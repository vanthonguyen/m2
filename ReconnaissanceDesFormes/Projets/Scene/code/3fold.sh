# !/bin/sh

echo > result
for c in 10 100 200 300 400 500
do
    for gamma in 0.005 0.05 0.1 0.5 2.0
    do
        echo "-------------$c-$gamma-------------">> result
        for i in {0..2}
        do
            echo ./training learning_3_$i vocabulary.xml svm.xml $c $gamma >> result
            ./training learning_3_$i vocabulary.xml svm.xml $c $gamma >> result
            ./classifier test_3_$i >> result
        done
    done
done
