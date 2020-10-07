#!/usr/bin/python3
import os
import glob

cwd=os.getcwd()

totalResultsFile = open("Totals", "a")

numOfFiles = 0
resultsFileName=""

nDict = {}

""" for file in os.listdir(cwd):
    if file.endswith(".csv"):
        resultsFileName = os.path.splitext(file)[0]
        resultsFileName = "Extracted-" + resultsFileName +".csv"
        outFile = open(resultsFileName, "w")
        with open(file, "r") as inFile:
            for eachLine in inFile:
                tempLine = list(eachLine.split(","))
                if tempLine[1] in nDict.keys():
                    if tempLine[3] > nDict.get(tempLine[1]):
                        nDict[tempLine[1]] = tempLine[3]
                else:
                    nDict.update({tempLine[1]:tempLine[3]})
            for entry in nDict:
                outFile.write(str(entry.keys()) + "," + entry.values())
            outFile.close() """


with open("Extracted-outputFixedProc.csv", "w") as outFile:
    with open("outputFixedProc.csv", "r") as inFile:
        for eachLine in inFile:
            tempLine = list(eachLine.split(","))
            if tempLine[1] in nDict.keys():
                if tempLine[3] > nDict.get(tempLine[1]):
                    nDict[tempLine[1]] = tempLine[3]
            else:
                nDict.update({tempLine[1]:tempLine[3]})
        for key, value in sorted(nDict.items(), key=lambda x: x[0]): 
            outFile.write("{},{}".format(key,value))
        inFile.close()
    outFile.close()
