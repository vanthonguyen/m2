# !/bin/sh
f=dic
#echo > $f 
for ds in 10 100 2000 5000 10000
do
    echo "-------------$ds-------------">> $f 
    for i in {0..2}
    do
        echo ./training learning_3_$i vocabulary.xml svm.xml 300 0.5 $ds >> $f
        ./training learning_3_$i vocabulary.xml svm.xml 300 0.5 $ds>> $f
        ./classifier test_3_$i vocabulary.xml svm.xml $ds >> $f
    done
done
