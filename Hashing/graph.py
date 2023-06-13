from matplotlib import pyplot as plt
import numpy as np



def readAndGraph(file, expectedCount, bitList, attack, saveFile, varSaveFile, graphTitle):
    with open (file) as f:
        lines = f.readlines()
        f.close()
    xList = []
    yList = []
    curYList = []
    lists = []

    y = 0
    curBit = -1
    bitCount = 0
    yAverageList = []

    yAverage = 0
    count = 0
    for line in lines:
        if line[:9] == "ATTEMPTS:":
            y = int(line[9:])
            xList.append(curBit)
            yList.append(y)
            curYList.append(y)
            yAverage = int(y) + yAverage
            count+=1
        elif line[:5] == "BITS:":
            curBit = int(line[5:])
            curYList.clear()
            bitCount+=1
    
        if (count > 1 and count % 50 == 0):
            yAverage = yAverage/50
            yAverageList.append(yAverage)
            yAverage = 0
            count = 0
            lists.append(curYList)
            curYList = []




    print(yAverageList)
    plt.plot(bitList, yAverageList, linestyle='--', marker='o', color='red', label="tested")
    plt.plot(bitList, expectedCount, marker='o', color='blue', label="expected")
    plt.legend()
    plt.title(attack)
    plt.xlabel('Bit Sizes')
    plt.ylabel('Average Attempts Over 50 iterations')
    plt.yscale("log")
    plt.xticks(bitList, bitList)
    plt.savefig(saveFile)
    plt.close()

    plt.boxplot(lists)
    plt.xticks([1, 2, 3, 4,5, 6, 7, 8], [8, 10, 12, 14, 16, 17, 18, 19])
    plt.title(graphTitle)
    plt.xlabel('Bit Sizes')
    plt.ylabel('Attempts to Break')
    plt.yscale("log")
    
    plt.savefig(varSaveFile)
    plt.close()





attack = "Pre-Image Attack Results"
expectedCount = [256, 1024, 4096, 16384, 65536, 131072, 262144, 524288]
bitList = [8, 10, 12, 14, 16, 17, 18, 19]
readAndGraph("PreImageResults.txt", expectedCount, bitList, attack, "PreImageGraph.jpg", "PreImageVariance.jpg", "Variance in Pre-Image Attack")


attack = "Collision Attack Results"
expectedCount = [16, 32, 64, 128, 256, 362, 512, 724]
readAndGraph("CollisionResults.txt", expectedCount, bitList, attack, "CollisionGraph.jpg" , "CollisionVariance.jpg", "Variance in Collision Attack")