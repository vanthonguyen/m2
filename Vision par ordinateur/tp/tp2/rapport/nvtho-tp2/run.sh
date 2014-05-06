#!bin/bash
for i in {"$1".."$2"}
do
    echo ./matchAll.sh coil-100pgmtest/obj$i"__"\* coil-100pgmrefkey
    #./matchAll.sh coil-100pgmtest/obj$i__\* coil-100pgmrefkey
done
