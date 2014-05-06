# !/bin/bash
tmpFile=/tmp/resultNumber
cp finalResult $tmpFile 
sed -i 's/obj//g' $tmpFile
cat $tmpFile|sort -n > finalResultSorted
