import re
import sys
#classes[]
classesOccurence = {}
classes = {}
inconnuImage = ""
finalResultFile = "finalResult" + str(sys.argv[2])
with open (str(sys.argv[1])) as bestMatch:
    for match in bestMatch:
        token = match.split(' ');
        classe = re.sub(r"__[^_]*", "", token[1])
        score = token[2]
        if inconnuImage == "":
            searchResult = re.search(r"(obj[^_]*)", token[0])
            if searchResult:
                inconnuImage = searchResult.group(1) 
        if classes.has_key(classe):
            classes[classe] = classes[classe] + float(score)
        else:
            classes[classe] = float(score)

        if classesOccurence.has_key(classe):
            classesOccurence[classe] = classesOccurence[classe] + 1
        else:
            classesOccurence[classe] = 1 

#recouperer 2 meilleur modeles
found1 = ""
found2 = ""
found1Value = 0
found2Value = 0
bestClass = ""
tp = 0
fp = 0
fn = 0
tn = 0
for c in classes.keys():
    if classesOccurence[c] > found1Value:
        found2 = found1
        found2Value = found1Value
        found1 = c
        found1Value = classesOccurence[c]
if found2 == "" or classesOccurence[found1] - classesOccurence[found2] > 2:
    bestClass = found1;
else:
    if classes[found1]/classesOccurence[found1] > classes[found2]/classesOccurence[found2]:
        bestClass = found1
    else:
        bestClass = found2
#write le result vers un fichier
fileResult = open(finalResultFile, "a+")
fileResult.write('{0} {1}\n'.format(inconnuImage,bestClass))
fileResult.close()
