# !/bin/sh
#create database for k-fold cross validation
k=$1
#database of images
db=../db

#function to shuffle a list
shuffle() {
    local i tmp size max rand
    size=${#images[*]}
    max=$(( 32768 / size * size ))

    for ((i=size-1; i>0; i--)); do
        while (( (rand=$RANDOM) >= max )); do :; done
        rand=$(( rand % (i+1) ))
        tmp=${images[i]} images[i]=${images[rand]} images[rand]=$tmp
    done
}       

scene_id=0
echo > scene_name_id 

learningPrefix=learning_$k"_"
testPrefix=test_$k"_"

#delete csv files
rm -f $learningPrefix*
rm -f $testPrefix*

numberOfTestClass=13
for subdir in `ls $db | head -n $numberOfTestClass`
do
    if [ -d $db/$subdir ] 
    then
        #maping name of scene to id
        images=($db/$subdir/*.jpg);
        #shuffle images
        shuffle 
        let scene_id=$scene_id+1 
        echo $subdir $scene_id >> scene_name_id
        size=${#images[*]}
        #limite 60 images/one cat
        #size=120
        let "partitionSize=$size/$k"

        for ((index=0; index<$k; index++))
        do
            echo $index
            learningFile=$learningPrefix$index
            testFile=$testPrefix$index
            let index1=$index+1
            let minTestIndex=$index*$partitionSize
            let maxTestIndex=$index1*$partitionSize
            if [ $index1 -eq $k ]
            then
                let maxTestIndex=$size
            fi

            imageIndex=0
            for image in "${images[@]}"
            do
                if [ $imageIndex -ge $minTestIndex ] && [ $imageIndex -lt $maxTestIndex ]
                then
                    echo $image";"$scene_id >>$testFile
                elif [ $imageIndex -lt $size ]
                then
                    echo $image";"$scene_id >>$learningFile
                fi
                let imageIndex=$imageIndex+1
            done
        done
    fi    
done 
