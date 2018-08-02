//
//  main.cpp
//  CSC 323 Project 7
//
//  Created by Joel Morel on 11/5/17.
//  Updated on 5/7/18
//  Copyright © 2018 Joel Morel. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

ifstream inFile;
ofstream outputFile1, outputFile2;

class DijktraSSS
{
private:
public:
     int numNodes;
     int sourceNode;
     int minNode;
     int currentNode;
     int newCost;
     int**  costMatrix;
     int* fatherAry;
     int* markedAry;
     int* bestCostAry;
    
    DijktraSSS(int nNodes, int sNode)
    {
        numNodes = nNodes + 1;
        sourceNode = sNode;
        minNode = 0;
        currentNode = 0;
        newCost = 0;
        
        costMatrix = new int*[numNodes];
        for(int i = 0; i <= numNodes; i++)
        {
            costMatrix[i] = new int[numNodes];
        }
        
        for(int i = 1; i < numNodes; i++)
        {
            for(int j = 1; j < numNodes; j++)
            {
                if(i == j)
                    costMatrix[i][i] = 0;
                else
                    costMatrix[i][j] = 99999;
            }
        }
       
        fatherAry = new int[numNodes];
        for(int i = 1; i < numNodes; i++)
        {
            fatherAry[i] = i;
        }
        
        markedAry = new int[numNodes];
        for(int i = 1; i < numNodes; i++)
        {
            markedAry[i] = 0;
        }

        bestCostAry = new int[numNodes];
        for(int i = 1; i < numNodes; i++)
        {
            bestCostAry[i] = 99999;
        }
        
        //Step 1
        loadCostMatrix();
        
        do{
        //Step 2
        setBestCostAry(sourceNode);
        setFatherAry(sourceNode);
        setMarkedAry(sourceNode);
        
        //Step 3: minNode = find an *unmarked* node with  minimum cost from bestCostAry
        do{
        minNode = findMinNode();
        markMinNode(minNode);
        debugPrint();

        if(minNode <= 0)
        {
            break;
        }

        //Step 4: expanding the min nod
            
        for(int i = 1; i < numNodes; i++)
        {
            currentNode = i;
            if(markedAry[i] == 1)
                continue;
            
            newCost = computeCost(minNode, currentNode);
            
            if(newCost < bestCostAry[currentNode])
            {
                changeFather(currentNode, minNode);
                changeCost(currentNode, newCost);
                
                debugPrint();
            }
        }   //Step 5: repeat step 4 until all unmarked node in markedAry are evaluated
        
        }while(true);    //Step 6: repeat step 3 to step 5  until all nodes are marked
        
        //Step 7: currentNode = 1
        currentNode = 1;

        //Step 8:  printShortestPath (currentNode)
        
        outputFile1 << "Source node is: " << sourceNode << endl;

        while(currentNode < numNodes)
        {
            printShortestPaths (currentNode);
        
        //Step 9:  currentNode ++
            currentNode++;
        
        }//Step 10: repeat step 8 and step 9 until currentNode >= numNodes
            outputFile1 << endl;
            sourceNode++;
        }
        while(sourceNode < numNodes);       // Step 11:  repeat step 2 to step 10 until sourceNode > numNodes
        
    }
    
    void loadCostMatrix()
    {
        int source, dest, cost;
        while(inFile >> source)
        {
            inFile >> dest >> cost;
            costMatrix[source][dest] = cost;
        }
        
        cout << "Cost matrix: " << endl;
        for(int i = 1; i < numNodes; i++)
        {
            for(int j = 1; j < numNodes; j++)
            {
                cout << costMatrix[i][j] << " ";
            }
            cout << endl;
        }

    }
    
    void setBestCostAry (int sourceNode)
    {
        for(int i = 1; i < numNodes; i++)
        {
            bestCostAry[i] = costMatrix[sourceNode][i];
        }
    }
    
    void setFatherAry(int sourceNode)
    {
        for(int i = 1; i < numNodes; i++)
        {
            fatherAry[i] = sourceNode;
        }
    }
    
    void setMarkedAry(int sourceNode)
    {
        
        for(int i = 1; i < numNodes; i++)
        {
            if(i == sourceNode)
            {
                markedAry[i] = 1;
            }
            else{
                
            markedAry[i] = 0;
            }
        }
    }
    
    int findMinNode()
    {
        int lowestCost = 99999;
        int minNode = 0;
        
        for(int i = 1; i < numNodes; i++)
        {
            if(bestCostAry[i] < lowestCost && markedAry[i] == 0)
            {
                minNode = i;
                lowestCost = bestCostAry[minNode];
            }
        }
        
        return minNode;
    }

    int computeCost (int minNode, int currentNode)
    {
        return (bestCostAry[minNode] + costMatrix[minNode][currentNode]);
    }
    
    void markMinNode (int minNode)
    {
        markedAry[minNode] = 1;
    }
    
    void changeFather (int node, int minNode)
    {
        fatherAry[node] = minNode;
    }
    
    void changeCost(int node, int newCost)
    {
        bestCostAry[node] = newCost;
    }
    
    void debugPrint()
    {

            outputFile2 << "The source node is: " << sourceNode << endl;
            
            outputFile2 << "Father array: ";
            for(int i = 1; i < numNodes; i++)
            {
                outputFile2 << fatherAry[i] << " ";
            }
        
            outputFile2 << endl;
            
            outputFile2 << "Best Cost array: ";
            for(int i = 1; i < numNodes; i++)
            {
                outputFile2 << bestCostAry[i] << " ";
            }
            outputFile2 << endl;
            
            outputFile2 << "Marked array: ";
            for(int i = 1; i < numNodes; i++)
            {
                outputFile2 << markedAry[i] << " ";
            }
            outputFile2 << endl;
            outputFile2 << "-----------------------" << endl;
        
    }

    void printShortestPaths (int currentNode)
    {
        int* pathArray = new int[numNodes];
        int temp = currentNode;
        int count = 1;
        int cost = 0;
        pathArray[1] = currentNode;
        for(int i = 2; i < numNodes; i++)
        {
            pathArray[i] = fatherAry[temp];
            temp = fatherAry[temp];
            count++;
            cost = bestCostAry[currentNode];
            if(pathArray[i] == sourceNode)
            {
                break;
            }
        }
    
        outputFile1 << "The path from " << sourceNode << " to " << currentNode <<": ";
        
        int i = count;
        while(i > 1)
        {
             outputFile1 << pathArray[i] << "-->";
            i--;
        }
        
        outputFile1 << pathArray[1] << " at cost: " << cost << endl;
    }
};

int main(int argc, const char * argv[])
{
    //Step 0
    inFile.open(argv[1]);
    outputFile1.open(argv[2]);
    outputFile2.open(argv[3]);
    
    int numNodes = 0, sourceNode = 1;
    
    if(inFile.is_open())
    {
        inFile >> numNodes;
    }
    
    DijktraSSS* driver = new DijktraSSS(numNodes, sourceNode);
    
    //Step 12
    inFile.close();
    outputFile1.close();
    outputFile2.close();
    
}

