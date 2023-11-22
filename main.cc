/**
 * @file main.cc
 * 
 * 
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <climits>

using namespace std;



void printGraph(vector<vector<int>> graph);
vector<int> parseLine(string line);
int minKey(vector<int> key, vector<bool> mstSet);
vector<int> primsAlgorithm(vector<vector<int>> graph);
void printMST(vector<int> parent, vector<vector<int>> graph);

int main(int argc, char const *argv[])
{
    string filename = "./graphs/graph1.txt";
    ifstream input(filename);
    vector<vector<int>> graph;
    vector<int> temp;
    string line;

    if (!input.is_open())
    {
        cout << "Could not open the file - '" << filename << "'" << endl;
        return 1;
    }
    getline(input, line);
    int numVerts = stoi(line);
    while (input)
    {
        getline(input, line);
        temp = parseLine(line);
        graph.push_back(temp);
    }

    input.close();
    
    vector<int> parent = primsAlgorithm(graph);
    printMST(parent, graph);


    return 0;
}


void printMST(vector<int> parent, vector<vector<int>> graph) {
    cout << "Edge \tWeight\n";
    for (size_t i = 1; i < graph.size(); i++)
        cout << parent[i] << " - " << i << " \t" << graph[i][parent[i]] << " \n";
}

vector<int> parseLine(string line)
{
    vector<int> result;
    string temp = "";
    int index = line.find(",");
    

    while (index != -1)
    {
        temp = line.substr(0, index);
        result.push_back(stoi(temp));
        line = line.substr(index + 1);
        index = line.find(",");
    }
    temp = line.substr(0);
    result.push_back(stoi(temp));
    return result;
}

void printGraph(vector<vector<int>> graph)
{
    for (size_t i = 0; i < graph.size(); i++)
    {
        cout << i << ": ";
        for (size_t j = 0; j < graph[i].size(); j++)
        {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
}


int minKey(vector<int> key, vector<bool> mstSet) {
    int min = INT_MAX, min_index;
    for (size_t v = 0; v < key.size(); v++)
        if (mstSet[v] == false && key[v] < min)
            min = key[v], min_index = v;
    return min_index;
}

vector<int> primsAlgorithm(vector<vector<int>> graph) {
    int V = graph.size();
    vector<int> parent(V);
    vector<int> key(V, INT_MAX);
    vector<bool> mstSet(V, false);

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < V - 1; count++) {
        int u = minKey(key, mstSet);
        mstSet[u] = true;
        for (int v = 0; v < V; v++)
            if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v])
                parent[v] = u, key[v] = graph[u][v];
    }
    return parent;
}