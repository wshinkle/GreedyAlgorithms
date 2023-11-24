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
#include <filesystem>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;
using namespace std::chrono;

int minKey(vector<int> key, vector<bool> mstSet, int vertices)
{
    // Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < vertices; v++)
        if (mstSet[v] == false && key[v] < min)
            min = key[v], min_index = v;

    return min_index;
}

void printMST(vector<vector<int>> mst, int vertices, string filename)
{
    filename = filename.substr(filename.find_last_of("/"));
    filename = filename.substr(0, filename.rfind("."));
    ofstream output("output/" + filename + "_out_prims.csv", ios::out);
    cout << "Vertex1,Vertex2,Weight\n";
    output << "Vertex1,Vertex2,Weight\n";
    for (size_t i = 0; i < mst.size(); i++)
    {
        cout << mst[i][0] << "," << mst[i][1] << "," << mst[i][2] << endl;
        output << mst[i][0] << "," << mst[i][1] << "," << mst[i][2] << endl;
    }
}
//O(ElogV)
vector<vector<int>> primsAlgorithm(vector<vector<int>> graph, int vertices)
{
    vector<vector<int>> result;

    vector<int> parent(vertices);

    vector<int> key(vertices, INT_MAX);

    vector<bool> mstSet(vertices, false);

    key[0] = 0;

    parent[0] = -1;

    for (int count = 0; count < vertices - 1; count++) // O(v^2)
    {

        int u = minKey(key, mstSet, vertices);

        mstSet[u] = true;

        for (int v = 0; v < vertices; v++) // O(v)

            if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v])
                parent[v] = u, key[v] = graph[u][v];
    }

    for (int i = 1; i < vertices; i++) //O(v)
    {
        result.push_back({parent[i] + 1, i + 1, graph[i][parent[i]]});
    }
    return result;
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

void printMatrx(vector<vector<int>> matrix)
{

    cout << "Adjacency Matrix: " << endl;
    for (size_t i = 0; i < matrix.size(); i++)
    {
        for (size_t j = 0; j < matrix[i].size(); j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main(int argc, char const *argv[])
{
    string path = "graphs";
    ifstream input;
    vector<int> temp;
    string line;
    string filename;
    vector<vector<int>> result;
    ofstream output("prims_output.csv", ios::out);
    output << "Experiment,Duration(microseconds)" << endl;
    for (const auto &entry : fs::directory_iterator(path))
    {
        cout << entry.path() << endl;
        filename = entry.path();
        input.open(filename);
        if (!input.is_open())
        {
            cout << "Could not open the file - '" << filename << "'" << endl;
            return 1;
        }
        getline(input, line);
        int numVerts = stoi(line);
        vector<vector<int>> graph(numVerts, vector<int>(numVerts, 0));

        while (input)
        {

            getline(input, line);
            if (line == "")
            {
                continue;
                ;
            }
            temp = parseLine(line);
            graph[temp[0] - 1][temp[1] - 1] = temp[2];
            graph[temp[1] - 1][temp[0] - 1] = temp[2];
        }

        input.close();
        // printMatrx(graph);
        string experimentName = filename.substr(filename.find_last_of("/") + 1);
        experimentName = experimentName.substr(0, experimentName.rfind("."));

        auto start = high_resolution_clock::now();
        result = primsAlgorithm(graph, numVerts);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        output << experimentName << "," << duration.count() << endl;
        printMST(result, numVerts, filename);
    }
    output.close();
    return 0;
}
