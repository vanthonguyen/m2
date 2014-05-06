# !/bin/sh

function blur {
    #prefix=${1%.pgm}
    #for kernelSize in {3..11..4}
    for kernelSize in {3..5..2}
    do
        ./blur $1 $kernelSize &
    done
}

learningDir=att_learning
if [ -d $learningDir ]
then
    rm -fr $learningDir
fi

echo "mkdir $learningDir"
mkdir $learningDir

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

        for image in {1..5}
        do
            originalImage=$1/$subdir/$image.pgm
            outputImagePrefix=$learningDir/$subdir/$image
            
            #copy image original
            echo cp $originalImage $outputImagePrefix.pgm 
            cp $originalImage $outputImagePrefix.pgm
            #blur original image
            blur $outputImagePrefix.pgm

            #modify contrast - histogram equalisation
            #equalisationImage=$outputImagePrefix"_eq.pgm"
            #./contrast $originalImage equalisation $equalisationImage 
            #blur $equalisationImage 

            #linear modify
            #alpha = 1.0 -> 3.0
            #beta = 0 -> 100
            #linear1=$outputImagePrefix"_l1.pgm"
            #./contrast $originalImage linear $linear1 1.2 50 #more light
            #blur $linear1

            #linear2=$outputImagePrefix"_l2.pgm"
            #./contrast $originalImage linear $linear2 0.4 10 #more light
            #blur $linear2

            #non linear - gamma correction
            #nonlinear1=$outputImagePrefix"_n1.pgm"
            #./contrast $originalImage nonlinear $nonlinear1 2 #more light
            #blur $nonlinear1

            #nonlinear2=$outputImagePrefix"_n2.pgm"
            #./contrast $originalImage nonlinear $nonlinear2 4 #more light
            #blur $nonlinear2

            #nonlinear3=$outputImagePrefix"_n3.pgm"
            #./contrast $originalImage nonlinear $nonlinear3 0.7 #more light
            #blur $nonlinear3

            #nonlinear4=$outputImagePrefix"_n4.pgm"
            #./contrast $originalImage nonlinear $nonlinear4 0.5 #more light
            #blur $nonlinear4
            #gaussian blur

        done
    fi    
done 
