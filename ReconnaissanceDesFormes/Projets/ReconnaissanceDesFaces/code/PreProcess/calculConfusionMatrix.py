import re
import sys
import math

result = {}
confusionMatrix="confusionMatrix"    
with open (str(sys.argv[1])) as matchResult:
    for match in matchResult:
        token = match.split(' ');
        index = '{0}-{1}'.format(token[0],token[1])
        if result.has_key(index):
            result[index] = result[index] + 1;
        else:
            result[index] = 1;

for x in result.keys():
    print x, result[x]
fileResult = open(confusionMatrix, "a+")
for i in range(1,101):
    fileResult.write('{0} '.format(i))

fileResult.write('\n')


for i in range(1,101):
    for j in range(1,101):
        index = '{0}-{1}'.format(i,j)
        if result.has_key(index):
            fileResult.write('{0} '.format(result[index]))
        else:
            fileResult.write('0 ')
    fileResult.write('\n')
fileResult.close()
