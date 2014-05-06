echo >descriptors
echo >test_descriptors
./training ../DigitData/testimages.txt test_descriptors
./training ../DigitData/trainingimages.txt descriptors
time ./svm ../DigitData/traininglabels.txt test_descriptors 

