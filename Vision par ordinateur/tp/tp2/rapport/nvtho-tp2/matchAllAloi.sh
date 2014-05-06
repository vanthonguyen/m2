#! /bin/bash
set -o nounset                              # Treat unset variables as an error
if [[ $# -ne 2 ]]
then
    echo "utilise ./matchAll.sh repertoire1/\* repertoire2"
    exit 0
fi
# $1 la liste des images de test
# $2 le repertoire des keys de sift de base de donnees d'apprentisage

#faire vide le fichier contenant le resultat final
#echo > finalResult 

#lire le repertoire des images de test "$1"
echo $1
for image in `ls $1`
do
    #matching chaque image
    echo $image
    ./matchaloi.sh $image $2
done
