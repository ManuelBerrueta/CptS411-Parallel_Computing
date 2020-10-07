#!/usr/bin/python3
import os
import glob

cwd=os.getcwd()

""" for file in os.listdir(cwd):
    print(file) """

i = 0

""" while(i < 12):
    with open("Results"+str(i)+".txt", "w") as tempFile:
        tempFile.write("Results"+str(i) +" Test\n")
        print(glob.glob("*.txt"))
    i+=1 """

totalResultsFile = open("Totals", "a")

numOfFiles = 0
resultsFileName=""

for file in os.listdir(cwd):
    if file.endswith(".txt"):
        if(numOfFiles == 0):
            resultsFileName = os.path.splitext(file)[0]
            resultsFileName = resultsFileName[:-1]
        numOfFiles += 1
        #print(file)
        #with open(file) as tempFile:
        #    #print(tempFile.read())
        #    totalResultsFile.write(tempFile.read())
    #totalResultsFile.write("\n")        
#totalResultsFile.close()
i=0
while(i < numOfFiles):
    with open(resultsFileName+str(i)+".txt", "r") as inFile:
        totalResultsFile.write(inFile.read())
    i+=1

totalResultsFile.close()

print("Finished appending " + str(numOfFiles) + " in fileName: Totals")