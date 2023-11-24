/**
 * @file kruskals.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-11-24
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
#include <set>

using namespace std;
namespace fs = std::filesystem;
using namespace std::chrono;




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

void sortGraph(vector<vector<int>> &graph)
{
    int temp;
    for (size_t i = 0; i < graph.size(); i++)
    {
        for (size_t j = 0; j < graph.size() - i - 1; j++)
        {
            if (graph[j][2] > graph[j + 1][2])
            {
                temp = graph[j][2];
                graph[j][2] = graph[j + 1][2];
                graph[j + 1][2] = temp;
            }
        }
    }
}


int findSet(vector<set<int>> sets, int vertex)
{
    for (size_t i = 0; i < sets.size(); i++)
    {
        if (sets[i].find(vertex) != sets[i].end())
        {
            return i;
        }
    }
    return -1;
}

void printMST(vector<vector<int>> mst, int vertices, string filename)
{   
    filename = filename.substr(filename.find_last_of("/"));
    filename = filename.substr(0, filename.rfind("."));
    ofstream output("output/" + filename + "_out_kruskals.csv", ios::out);
    cout << "Vertex1,Vertex2,Weight\n";
    output << "Vertex1,Vertex2,Weight\n";
    for (size_t i = 0; i < mst.size(); i++)
    {
        cout << mst[i][0] << "," << mst[i][1] << "," << mst[i][2] << endl;
        output << mst[i][0] << "," << mst[i][1] << "," << mst[i][2] << endl;
    }
}


//O(EloV)
vector<vector<int>> kruskalsAlgorithm(vector<vector<int>> graph, int vertices)
{
    sortGraph(graph);
    vector<vector<int>> result;
    vector<set<int>> sets;
    int u,v, weight, indexU, indexV;
    for (int i = 0; i < vertices; i++)
    {
        set<int> temp;
        temp.insert(i + 1);
        sets.push_back(temp);
    }

    for (size_t i = 0; i < graph.size(); i++)
    {
        u = graph[i][0];
        v = graph[i][1];
        weight = graph[i][2];
        indexU = findSet(sets, u);
        indexV = findSet(sets, v);
        if (indexU != indexV)
        {
            result.push_back({u, v, weight});
            sets[indexU].insert(sets[indexV].begin(), sets[indexV].end());
            sets.erase(sets.begin() + indexV);
            if(sets.size() == 1){
                break;
            }
        }
    }
    return result;

}



int main()
{
    string filename;
    ifstream input;
    ofstream output("kruskals_output.csv", ios::out);
    string path = "./graphs";
    string line;
    vector<int> temp;
    vector<vector<int>> result;
    output << "Experiment,Durations (microseconds)" << endl; 
    for (const auto &entry : fs::directory_iterator(path))
    {
        filename = entry.path();
        input.open(filename, ios::in);
        if (!input.is_open())
        {
            cout << "File " << filename << " failed to open." << endl;
            return 1;
        }
        getline(input, line);
        int vertices = stoi(line);
        vector<vector<int>> graph;
        while (input)
        {

            getline(input, line);
            if (line == "")
            {
                continue;
                
            }
            temp = parseLine(line);
            graph.push_back(temp);
        }
        input.close();

        string experimentName = filename.substr(filename.find_last_of("/") + 1);
        experimentName = experimentName.substr(0, experimentName.rfind("."));


        cout << "Running Kruskal's Algorithm on " << filename << endl;
        auto start = high_resolution_clock::now();
        result = kruskalsAlgorithm(graph, vertices);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        output << experimentName << "," << duration.count() << endl;
        printMST(result, vertices, filename);
    }
    output.close();
    return 0;
}