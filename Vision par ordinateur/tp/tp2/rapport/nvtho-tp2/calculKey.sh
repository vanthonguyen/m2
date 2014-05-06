set -o nounset                              # Treat unset variables as an error
#input $1
echo mkdir $1"key"
if [ ! -e $1"key" ] 
then
    mkdir $1"key"
fi

for image in `ls $1`
do
    base="${image%.[^.]*}"
    key=$base".key"
    echo ./sift < $1/$image > $1"key/"$key
    ./sift < $1/$image > $1"key/"$key
done
