/*
PROJECT NAME:
Implementation CLASS: Analysis and Design of Advanced Algorithms, TC2038
STUDENTS: Ana Karen Valencia Villaseñor | A01067716
          Sergio Eduardo Gutiérrez Torres | A01068505
          Sebastián Cervera Maltos | A01068
PROFESSOR: Gildardo Sánchez Ante
DATE: 05/11/24
Description:

*/

#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const int INF = 999999999;

bool is_valid(int a, vector<int> vector) {
    for (int i = 0; i < vector.size(); i++) {
        if (a == vector[i]) {
            return false;
        }
    }
    return true;
}

vector<vector<int>> MTS(vector<vector<int>>& graph, int n) {
    vector<int> visited;
    visited.push_back(0);
    vector<vector<int>> coordenates;
    while (visited.size() < n) {
        int currentMin = INF;
        vector<int> index = { -1, -1 };
        for (int i = 0; i < visited.size(); i++) {
            for (int j = 0; j < graph[visited[i]].size(); j++) {
                if (graph[visited[i]][j] != -1 && graph[visited[i]][j] < currentMin &&
                    is_valid(j, visited)) {
                    currentMin = graph[visited[i]][j];
                    index[0] = visited[i];
                    index[1] = j;
                }
            }
        }
        coordenates.push_back(index);
        visited.push_back(index[1]);
    }
    return coordenates;

};

// Function to find the route using the nearest neighbor algorithm
int nearestNeighbor(const vector<vector<int>>& dist, vector<int>& path) {
    int n = dist.size();
    vector<bool> visited(n, false);
    int totalCost = 0;
    int current = 0; // It starts from city 0
    path.push_back(current); // Add the starting city to the route
    visited[current] = true;

    for (int i = 1; i < n; i++) {
        int nextCity = -1;
        int minCost = INF;


        for (int j = 0; j < n; j++) { // to find the nearest unvisited neighbor
            // cities that haven´t been visited and have a valid connection
            if (!visited[j] && dist[current][j] < minCost && dist[current][j] != INF) {
                minCost = dist[current][j];
                nextCity = j;
            }
        }

        if (nextCity != -1) {
            path.push_back(nextCity);// Adds the city to the route
            totalCost += minCost; // Adds the cost
            current = nextCity;  // Moves to the next one
            visited[current] = true; // Marks the current city as visited
        }
    }


    if (dist[current][0] != -1 && dist[current][0] != INF) {
        totalCost += dist[current][0];
        path.push_back(0);
    }
    else {
        cout << "Cannot return to the starting city." << endl;
    }

    return totalCost;
}

bool bfs(const vector<vector<int>>& residualGraph, int source, int sink,
    vector<int>& parent) {
    int N = residualGraph.size();
    vector<bool> visited(N, false);
    queue<int> q;

    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < N; v++) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                if (v == sink) {
                    parent[v] = u;
                    return true;
                }
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return false;
}

int fordFulkerson(const vector<vector<int>>& capacityMatrix, int source,
    int sink) {
    int N = capacityMatrix.size();
    vector<vector<int>> residualGraph = capacityMatrix;
    vector<int> parent(N);
    int maxFlow = 0;

    while (bfs(residualGraph, source, sink, parent)) {
        int pathFlow = INT_MAX;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = min(pathFlow, residualGraph[u][v]);
        }

        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}


int main() {

    ifstream infile("C:/Users/sergu/OneDrive/Documentos/Tec/5to_Semestre/Analysis and Design of Advanced Algorithms/TC2038/ActInt2/ActInt2/testcase1.txt");

    if (!infile) {
        cerr << "Unable to open Input File" << endl;
        return 1;
    }

    // Graphs sizes
    int n;
    infile >> n;

    // Load 1st Graph
    vector<vector<int>> graph1(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            infile >> graph1[i][j];
        }
    }

    // Load 2nd Graph
    vector<vector<int>> graph2(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            infile >> graph2[i][j];
        }
    }

    // Close the input file
    infile.close();
    cout << ">> Data loaded correctly" << endl;

    // ----------------------------- PART 1 -----------------------------

    vector<vector<int>> listOfArcs = MTS(graph1, n);
    for (int i = 0; i < listOfArcs.size(); i++) {
        cout << "(" << listOfArcs[i][0] << ", " << listOfArcs[i][1] << ") " << endl;
    }

    // ----------------------------- PART 2 -----------------------------

    vector<int> path;
    int totalCost = nearestNeighbor(graph1, path);

    cout << "Total cost: " << totalCost << endl;

    cout << "Most eficcient route : ";
    for (int city : path) {
        cout << city << " ";
    }
    cout << endl;

    // ----------------------------- PART 3 -----------------------------

    int source = 0;
    int sink = n - 1;

    int maxFlow = fordFulkerson(graph2, source, sink);
    cout << "El flujo máximo desde el nodo " << source << " hasta el nodo "
        << sink << " es: " << maxFlow << endl;

    // ----------------------------- PART 4 -----------------------------

    return 0;
}