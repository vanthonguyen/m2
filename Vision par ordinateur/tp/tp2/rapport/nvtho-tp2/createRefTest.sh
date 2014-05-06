set -o nounset                              # Treat unset variables as an error
#input $1
refDir=$1"ref"
testDir=$1"test"
if [ -d $refDir ]
then
    rm -fr $refDir
fi
if [ -d $testDir]
then
    rm -fr $testDir
fi

echo mkdir $refDir 
mkdir $refDir 
echo mkdir $testDir 
mkdir $testDir 
for subdir in `ls $1`
do
    if [ -d $1/$subdir ] 
    then
        #create subdir for output
        if [ ! -e $refDir/$subdir ]
        then
            echo mkdir $refDir/"$subdir
            mkdir $refDir/"$subdir
        fi

        if [ ! -e $testDir/$subdir ]
        then
            echo mkdir $testDir/"$subdir
            mkdir $testDir/"$subdir
        fi

        for image in `ls $1/$subdir`
        do
            #filename="${subdir##*/}"
            #separe les images en deux ensembles en meme taille, les images d'apprentisage et de test
            rnd=$((RANDOM % 2));
            if [ $rnd -eq 0 ]
            then
                echo cp $1/$subdir/$image $refDir/$subdir
                cp $1/$subdir/$image $refDir/$subdir
            else
                echo cp $1/$subdir/$image $testDir/$subdir
                cp $1/$subdir/$image $testDir/$subdir
            fi
        done
    else
        #1 test 5 reference 
        rnd=$((RANDOM % 2));
        if [[ $rnd -eq 0 ]]
        then
            echo cp $1/$subdir $1"test"/$subdir
            cp $1/$subdir $1"test"/$subdir
        else
            echo cp $1/$subdir $1"ref"/$subdir
            cp $1/$subdir $1"ref"/$subdir
        fi
    fi
done
