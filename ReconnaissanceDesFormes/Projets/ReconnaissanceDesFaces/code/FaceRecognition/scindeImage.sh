#!/bin/sh

# Suppression de l'executable
#rm -f faceRecognition

# Creation executable 
#make

# Suppression des fichiers si existants
rm -f test.csv
rm -f apprentissage.csv

# Creation des fichiers
touch test.csv
touch apprentissage.csv

# Phase pour scinder les images d'apprentissage 
# des images de test. Les 5 premiers sont des images 
# d'apprentissage et le reste les images de test

for k in `seq 1 40`
do
    i=0
    for file in att_faces/s$k/*.pgm
    do
        if [ "$i" -lt "5" ]
        then
            echo $file";"$k >> apprentissage.csv
        else
            echo $file";"$k >> test.csv
        fi
        #i=$(($i + 1)) 
        let i=$i+1
    done
done

# lancement de la premiere fonction de reconnaissance
# Methode 1
#for n in {0..91..91} 
#do
#    echo components $n
#    time ./faceRecognition apprentissage.csv test.csv 1 $n
#done
#time ./faceRecognition apprentissage.csv test.csv 1 $n
# Methode 2
#./faceRecognition apprentissage.csv test.csv 2

# Methode 3
#./faceRecognition apprentissage.csv test.csv 3

