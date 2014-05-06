set -o nounset                              # Treat unset variables as an error
#input $1
#output $2
for subdir in `ls $1`
do
    if [ -d $1/$subdir ] 
    then
        #create subdir for output
        if [ ! -e $2/$subdir ]
        then
            echo mkdir $2/$subdir
            mkdir $2/$subdir
        fi
        for image in `ls $1/$subdir`
        do
            #filename="${subdir##*/}"
            base="${image%.[^.]*}"
            output="$2/$subdir/$base.pgm";
            input="$1/$subdir/$image"
            echo convert $input $output
            convert $input $output
        done
    else
        #it is file
        base="${subdir%.[^.]*}"
        output="$2/$base.pgm";
        input="$1/$subdir"
        echo convert $input $output
        convert $input $output
    fi
done
