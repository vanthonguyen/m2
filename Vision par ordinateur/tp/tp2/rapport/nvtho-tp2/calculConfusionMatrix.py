import re
import sys
import math

result = {}
confusionMatrix=str(sys.argv[3])
matrixSize=int(sys.argv[1])
texFormatMatrix=confusionMatrix + "tex"
with open (sys.argv[2]) as matchResult:
    for match in matchResult:
        token = match.split(' ');
        index = '{0}-{1}'.format(token[0],int(token[1]))
        if result.has_key(index):
            result[index] = result[index] + 1;
        else:
            result[index] = 1;

fileResult = open(confusionMatrix, "a+")
#tpfn = [0 for x in range(1,matrixSize + 2)]
#tp = [0 for x in range(1,matrixSize + 2)]
#fp = [0 for x in range(1,matrixSize + 2)]
#fn = [0 for x in range(1,matrixSize + 2)]
#tn = [0 for x in range(1,matrixSize + 2)]
tp=0
fp=0
for i in range(1,matrixSize + 1):
    for j in range(1, matrixSize + 1):
        #tpfn[i] = tpfn[i] + 1;
        index = '{0}-{1}'.format(i,j)
        if result.has_key(index):
            if i == j:
                #tp[i] = tp[i] + 1;
                tp = tp + result[index] 
            else:
                #fn[i] = fn[i] + 1;
                #fp[j] = fp[j] + 1;
                fp = fp + result[index] 
            fileResult.write('{0} '.format(result[index]))
        else:
            fileResult.write('0 ')
    fileResult.write('\n')
print tp, fp, 1.0*tp/(tp + fp)
#to tex
#limit the number of classes, so it can be display in a pdf page
maxClass= 30
fileTex = open(texFormatMatrix, "a+")
#first row, header of table

fileTex.write('&')
for i in range(1,maxClass + 1):
    fileTex.write('{0}&'.format(i))
#terminate a line
fileTex.write('\\\\\n')
#drawline
fileTex.write('\\hline\n')
    
for i in range(1,maxClass + 1):
    #write row 
    fileTex.write('{0}&'.format(i))
    for j in range(1, maxClass + 1):
        index = '{0}-{1}'.format(i,j)
        if result.has_key(index):
            fileTex.write('{0}&'.format(result[index]))
        else:
            fileTex.write('0&')
    fileTex.write('\\\\\n\\hline\n')
fileTex.close()
fileResult.close()
