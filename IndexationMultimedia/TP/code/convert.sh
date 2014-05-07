#resize
for i in *.png; do convert -resize 620x $i $i ; done
#crop
for i in *.png; do convert -crop 620x860 $i $i;done
#rename
for i in *-0.png; do mv $i ${i%-0.png}.png;done
#remove 
rm -f *-1.png
