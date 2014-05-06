# !/bin/bash

#$1 number of temp file
im1=""
outDir="outaloi"
refkeyDir="aloirefkey"
refimDir="aloiref"
obj1=""
i=0
strName=""
strScore=""
while read line 
do
    if [[ -z $im1 ]]
    then
        im1=`echo $line | awk '{print $1}'`
        k1="/tmp/inconu$1".key
        obj1=${im1#*/}
        obj1=${obj1%.[^.]*}
        mkdir $outDir/$obj1
    fi
    im2=`echo $line | awk '{print $2}'`
    obj2=${im2%.[^.]*}
    k2=$obj2".key"
    let i++
    score=`echo $line | awk '{print $3}'`
    imout=$outDir/$obj1/"$i_$obj1-$obj2"
    ./match4 -im1 $im1 -k1 $k1 -im2 $refimDir/$im2 -k2 $refkeyDir/$k2 >$imout".pgm"
    convert -resize 128x192 $imout.pgm $imout.jpg
    echo "\includegraphics[width=1.5cm]{../images/$imout.jpg}&"
    strName=$strName"${obj2} &"
    #strScore=$strScore`echo scale=2;`
    strScore=$strScore"${score:0:4}&"
done < /tmp/bestMatch$1
strName=${strName//_/\\_}
echo $strName"\\\\"
echo "$strScore"
