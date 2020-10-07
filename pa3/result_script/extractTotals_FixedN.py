#!/usr/bin/python3
import os
import glob

cwd=os.getcwd()

totalResultsFile = open("Totals", "a")

numOfFiles = 0
resultsFileName=""

nDict = {}

with open("Extracted-outputFixedN.csv", "w") as outFile:
    with open("outputFixedN.csv", "r") as inFile:
        for eachLine in inFile:
            tempLine = list(eachLine.split(","))
            if tempLine[0] in nDict.keys():
                if tempLine[3] > nDict.get(tempLine[0]):
                    nDict[tempLine[0]] = tempLine[3]
            else:
                nDict.update({tempLine[0]:tempLine[3]})
        for key, value in sorted(nDict.items(), key=lambda x: x[0]): 
            outFile.write("{},{}".format(key,value))
        inFile.close()
    outFile.close()