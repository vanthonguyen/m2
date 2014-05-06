#! /bin/bash
set -o nounset                              # Treat unset variables as an error

goodCandidate=10
rnd=$((RANDOM))
inconnuKeyFile="/tmp/inconu$rnd.key"
resultatFile="/tmp/resultat$rnd"
matchFile="/tmp/bestMatch$rnd"
echo $inconnuKeyFile
echo $resultatFile
echo $matchFile 

#input $1
src="${1%.[^.]*}.pgm"

#creer un fichier vide pour stocker le resultat
echo > $resultatFile

#creer les keys sift pour l'image inconnue
echo "./sift < $1 > $inconnuKeyFile" 
./sift < $1 > $inconnuKeyFile 

for key in `ls $2`
do
    echo ./match2  -k1 $inconnuKeyFile -k2 $2/$key
    rs=`./match2  -k1 $inconnuKeyFile -k2 $2/$key`
    #recouperer le score
    m=`echo $rs|awk '{print $NF}'`
    #m=`echo $rs | sed '/Found/d' | awk '{print $2}'`
    bestMatchFile="${key%.[^.]*}.pgm"
    echo $src $bestMatchFile $m >> $resultatFile
done

#recouperer 30 meilleurs modeles
cat $resultatFile|sort -k3 -n -r|head -n10 > $matchFile
#classifier par calculer le score moyen pour chaque classe
python2 classifier.py $matchFile "_coil"
