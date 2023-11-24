/**
 * @file kruskals_opt.cc
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
    result[0] = result[0] - 1;
    result[1] = result[1] - 1;
    return result;
}

void sortGraph(vector<vector<int>> &graph) //O(n^2)
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

void printMST(vector<vector<int>> mst, int vertices, string filename)
{
    filename = filename.substr(filename.find_last_of("/"));
    filename = filename.substr(0, filename.rfind("."));
    ofstream output("output/" + filename + "_out_kruskals_opt.csv", ios::out);
    cout << "Vertex1,Vertex2,Weight\n";
    output << "Vertex1,Vertex2,Weight\n";
    for (size_t i = 0; i < mst.size(); i++)
    {
        cout << mst[i][0] + 1 << "," << mst[i][1] + 1 << "," << mst[i][2] << endl;
        output << mst[i][0] + 1 << "," << mst[i][1] + 1 << "," << mst[i][2] << endl;
    }
}

class dataStructure
{
    int *parent;

public:
    dataStructure(int n) //O(n)
    {
        parent = new int[n];

        for (int i = 0; i < n; i++)
        {
            parent[i] = i;
        }
    }

    // Find function
    int find(int i)
    {
        if (parent[i] != i)
            parent[i] = find(parent[i]);

        return parent[i];
    }

    // Union function
    void unite(int x, int y)
    {
        int s1 = find(x);
        int s2 = find(y);

        if (s1 != s2)
        {
            parent[s1] = s2;
        }
    }
};

class Graph
{
    vector<vector<int>> edgelist;
    int V;

public:
    Graph(int V) { this->V = V; }

    // Function to add edge in a graph
    void add(vector<int> edge)
    {

        edgelist.push_back(edge);
    }
    //O(ElogV)
    vector<vector<int>> kruskals_mst()
    {

        vector<vector<int>> result;

        // Sort all edges
        sortGraph(edgelist); //O(e^2)

        dataStructure s(V); //O(v)

        for (auto edge : edgelist) //O(e)
        {
            int x = edge[0];
            int y = edge[1];

            //Find + Union = O(VlogV)
            if (s.find(x) != s.find(y))
            {
                s.unite(x, y);
                result.push_back(edge);
            }
        }
        return result;
    }
};

int main()
{
    string filename;
    ifstream input;
    ofstream output("kruskals_opt_output.csv", ios::out);
    string path = "./graphs";
    string line;
    vector<int> temp;
    vector<vector<int>> result;
    output << "Kruskal's Algorithm Optimized" << endl;
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
        Graph graph(vertices);
        while (input)
        {

            getline(input, line);
            if (line == "")
            {
                continue;
            }
            temp = parseLine(line);
            graph.add(temp);
        }
        input.close();

        string experimentName = filename.substr(filename.find_last_of("/") + 1);
        experimentName = experimentName.substr(0, experimentName.rfind("."));

        cout << "Running Kruskal's Algorithm on " << filename << endl;
        auto start = high_resolution_clock::now();
        result = graph.kruskals_mst();
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        output << experimentName << "," << duration.count() << endl;
        printMST(result, vertices, filename);
    }
    output.close();
    return 0;
}
