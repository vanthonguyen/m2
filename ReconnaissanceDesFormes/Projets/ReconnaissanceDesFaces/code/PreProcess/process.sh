# !/bin/sh
echo $1

function blur {
    for kernelSize in {3..5..2}
    do
        ./blur $1 $kernelSize &
    done
}

shuffle() {
    local i tmp size max rand
    size=${#ids[*]}                                                                                                                                             
    max=$(( 32768 / size * size ))

    for ((i=size-1; i>0; i--)); do
        while (( (rand=$RANDOM) >= max )); do :; done
        rand=$(( rand % (i+1) ))
        tmp=${ids[i]} ids[i]=${ids[rand]} ids[rand]=$tmp
    done
}       
        
# Define the ids named 'ids'
ids=( 1 2 3 4 5 6 7 8 9 10)
        
shuffle 

function generate {

learningDir=att_learning$2
if [ -d $learningDir ]
then
    rm -fr $learningDir
fi

echo "mkdir $learningDir"
mkdir $learningDir

if [ $2 -eq 1 ]
then
    begin1=0
    end1=4
    begin2=5
    end2=9
else
    begin1=5
    end1=9
    begin2=0
    end2=4
fi
for subdir in `ls $1`
do
    if [ -d $1/$subdir ] 
    then
        #create subdir for output
        if [ ! -e $learningDir/$subdir ]
        then
            echo mkdir $learningDir/$subdir
            mkdir $learningDir/$subdir
        fi
        
        #5 first images of ids for training  
        #for index in {$begin1..$end1}
        for ((index=$begin1;index<=$end1;++index));
        do
            image="${ids[$index]}"
            originalImage=$1/$subdir/$image.pgm
            outputImagePrefix=$learningDir/$subdir/$image

            #copy image original
            echo cp $originalImage $outputImagePrefix.pgm 
            cp $originalImage $outputImagePrefix.pgm
            #blur original image
            blur $outputImagePrefix.pgm
        done
    fi    
done 

#generate file

testFile=test$2.csv
trainingFile=training$2.csv

trainingGaussianFile=trainingGaussian$2.csv
# Creation des fichiers
echo >$testFile
echo >$trainingFile
echo >$trainingGaussianFile

#generate test
for k in `seq 1 40`
do
    #for index in {$begin2..$end2}
    for ((index=$begin2;index<=$end2;++index));
    do
        image="${ids[$index]}"
        echo att_faces/s$k/$image.pgm";"$k >> $testFile 
    done
done

for k in `seq 1 40`
do
    #for index in {$begin1..$end1}
    for ((index=$begin1;index<=$end1;++index));
    do
        image="${ids[$index]}"
        echo att_faces/s$k/$image.pgm";"$k >> $trainingFile
    done
done

#training with blur images
for k in `seq 1 40`
do
    for file in $learningDir/s$k/*.pgm
    do
        echo $file";"$k >> $trainingGaussianFile
    done
done
}

generate $1 1
generate $1 2
