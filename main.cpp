/*
PROJECT NAME: Integrated Activity 2
Implementation CLASS: Analysis and Design of Advanced Algorithms, TC2038
STUDENTS: Ana Karen Valencia Villaseñor | A01067716
          Sergio Eduardo Gutiérrez Torres | A01068505
          Sebastián Cervera Maltos | A01068
PROFESSOR: Gildardo Sánchez Ante
DATE: 09/11/24
Description: Each test case file in this project contain one number "n" that represent
the number of nodes or the number of points, respectively; two different graphs of n*n,
the first for the distances between neighborhoods, and the second for max flow between
them; and a set of coordinates/points that represent a company's service provider HUB
or so. Program Requirements:

- Optimal Neighborhoods Connections: Connect a series of neighborhoods represented
in a weighted graph in the most optimal way (using MST algorithms).

- Optimal Travel Route: Establish the most optimal route that a deliveryman could
take to visit every neighborhood before returning to the starting one (using TSP
algorithms).

- Max Transmission Flow: The program should determine the max-flow for transmission
data from the first to the last node/neighborhood (using the Ford-Fulkerson algorithm).

- Service Coverage Area: Finally, is indicated to establish each company's center for
service provision area, delimitated by polygons (using a Voronoi's Diagram).
*/


/*
TEST CASES DESCRIPTION:


*/


// ----------------------------- IMPORTS -----------------------------

#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const int INF = 999999999;

struct Points
{
    int x;
    int y;
    Points (int x, int y) : x(x), y(y) {}
};


// ----------------------------- MST Prim's Algorithm -----------------------------

// Auxiliary function for verifying an already connected vertex.
bool isValid(int a, vector<int> vector) {
    for (int i = 0; i < vector.size(); i++) {
        if (a == vector[i]) {
            return false;
        }
    }

    return true;
}

// MST primary function to find the most optimal way to connect the neighborhoods.
// Complexity of O(E + V * log(V)), where V means the number of vertices and E the
// number of edges of the graph.
vector<vector<int>> mts(vector<vector<int>>& graph, int n) {
    vector<int> visited;    // To keep record of the already connected vertexes
    visited.push_back(0);   // The first vector
    vector<vector<int>> coordinates;    // To keep track of the route

    while (visited.size() < n) {
        int currentMin = INF;
        vector<int> index = { -1, -1 };

        for (int i = 0; i < visited.size(); i++) {
            for (int j = 0; j < graph[visited[i]].size(); j++) {
                // Next vertex choosing verification
                if (graph[visited[i]][j] != -1 && graph[visited[i]][j] < currentMin &&
                    isValid(j, visited)) {
                    currentMin = graph[visited[i]][j];
                    index[0] = visited[i];
                    index[1] = j;
                }
            }
        }
        // Add next vertex
        coordinates.push_back(index);
        visited.push_back(index[1]);
    }

    return coordinates;
};


// ----------------------------- Nearest Neighbor Algorithm -----------------------------

// Function to find the most optimal route using the nearest neighbor algorithm.
// Complexity of O(n^2), where "n" represents the number of nodes/vertices in the graph.
int nearestNeighbor(const vector<vector<int>>& dist, vector<int>& path) {
    int n = dist.size();    // Number of cities
    vector<bool> visited(n, false); // Keeps track of whether each city has been visited
    int totalCost = 0;  // Total cost of the journey
    int current = 0;    // Starting city
    path.push_back(current);
    visited[current] = true;

    for (int i = 1; i < n; i++) {   // Loop to visit all cities
        int nextCity = -1;  //Next city to visit
        int minCost = INF;  // Minimum cost to reach the next city

        for (int j = 0; j < n; j++) {   // Find the nearest unvisited neighbor
            // Check cities that haven't been visited and have a valid connection
            if (!visited[j] && dist[current][j] < minCost && dist[current][j] != INF) {
                minCost = dist[current][j]; // Update the minimum cost
                nextCity = j;   // Update the next city to visit
            }
        }

        if (nextCity != -1) {
            path.push_back(nextCity);   // Add the city to the route
            totalCost += minCost;   // Add the cost of traveling to the city
            current = nextCity;     // Move to the next city
            visited[current] = true;    // Mark the current city as visited
        }
    }

    // To return to the initial city
    if (dist[current][0] != -1 && dist[current][0] != INF) {
        totalCost += dist[current][0];  // Add the cost of returning to the initial city
        path.push_back(0);  // Add the initial city to complete the route
    } else {
        cout << "Cannot return to the starting city." << endl;
    }

    return totalCost;   // total cost of the journey
}


// ----------------------------- Ford-Fulkerson Algorithm -----------------------------

// Auxiliary function ...
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

// Function implementing Ford-Fulkerson algorithm to find the max-flow to transmit, from
// the source vertex to the sink vertex of the graph.
// Complexity of O(E * |f*|), where "E" is the number of different flow channels in the
// graph, and |f*| is the absolute value of the max-flow found in it.
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

// ----------------------------- Voronoi Diagram Algorithm -----------------------------

// Function to
// Complexity of O(n * log(n)), where "n" is the number of nodes/locations from where the
// designated areas are created

// ----------------------------- MAIN PROGRAM -----------------------------

int main() {

    // ---***--- FILE IMPORTS ---***---

    // Import info from each test case file.
    ifstream infile("C:/Users/sergu/OneDrive/Documentos/Tec/5to_Semestre/Analysis and Design of Advanced Algorithms/TC2038/ActInt2/IntAct2/testcase1.txt");

    if (!infile) {
        cerr << "Unable to open Input File" << endl;
        return 1;
    }

    // Graphs sizes/Points' number
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

    // Load points
    int x;
    int y;
    char c;
    vector<Points> points;
    for (int i = 0; i < n; i++) {
        infile >> c >> x >> c >> y>> c;
        points.push_back(Points(x, y));
    }

    // Close the input file
    infile.close();
    cout << ">> Data loaded correctly" << endl;


    // ---***--- PART 1 ---***---

    cout << "\n>> 1.- Optimal Neighborhoods Connections:" << endl;

    // Save the optimal route coordinates sequence
    vector<vector<int>> listOfArcs = mts(graph1, n);
    for (int i = 0; i < listOfArcs.size(); i++) {   // Print MST results
        cout << "(" << listOfArcs[i][0] << ", " << listOfArcs[i][1] << ") " << endl;
    }


    // ---***--- PART 2 ---***---

    cout << "\n>> 2.- Optimal Travel Route:" << endl;

    // Save the optimal travelling path sequence
    vector<int> path;
    int totalCost = nearestNeighbor(graph1, path);

    // Total cost for TSP
    // cout << "Total cost: " << totalCost << endl;

    // Print TSP results
    cout << "Most eficcient route : ";
    for (int city : path) {
        cout << city << " ";
    }
    cout << endl;


    // ---***--- PART 3 ---***---

    cout << "\n>> 3.- Max Transmission Flow:" << endl;

    // First and last nodes as source and sink vertexes directions inicializations
    int source = 0;
    int sink = n - 1;

    // Safe the max-transmission-flow
    int maxFlow = fordFulkerson(graph2, source, sink);

    // Print Ford-Fulkerson results
    cout << "Max-flow from node " << source << " to node "
        << sink << " is: " << maxFlow << endl;


    // ---***--- PART 4 ---***---

    cout << "\n>> 4.- Service Coverage Area:" << endl;
    //cout << "Los puntos son:" << endl;
    //for (const auto& point : points) {
    //    cout << "(" << point.x << ", " << point.y << ")" << endl;
    //}

    return 0;
}