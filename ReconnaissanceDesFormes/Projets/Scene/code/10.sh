# !/bin/sh
f=10scene
#echo > $f 
for i in {0..2}
do
    echo ./training learning_3_$i vocabulary.xml svm.xml 300 0.5 1000 >> $f
    ./training learning_3_$i vocabulary.xml svm.xml 300 0.5 1000>> $f
    ./classifier test_3_$i vocabulary.xml svm.xml 1000 >> $f
done
