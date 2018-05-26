//
//  main.cpp
//  1237-timus
//
//  Created by Bulat Galiev on 25.05.2018.
//  Copyright © 2018 Булат Галиев. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <limits>
using namespace std;

class construction { // Building or shelter
public:
    int x;
    int y;
    int capacity; // Number of people inside the building or capacity of the shelter
    
    int distance (const construction& other) {
        return abs(x - other.x) + abs(y - other.y) + 1;
    }
};

void initialize_network(std::vector<vector <int>>& flow, std::vector<vector <int>>& cost, std::vector<int>& inflow, int n, int m, vector <construction> B, vector <construction> C, int source, int sink) {
    // Calculating costs
    for (int i=0; i<n; i++) {
        flow[source][i] = B[i].capacity;
        for (int j=0; j<m; j++) {
            flow[i][n+j] = B[i].capacity;
            int currentDistance = B[i].distance(C[j]);
            cost[i][n+j] = currentDistance;
            cost[n+j][i] = -currentDistance;
        }
    }
    
    // Initializing residual network
    for (int i=0; i<n; i++){
        for (int j=0; j<m; j++){
            std::cin >> flow[n+j][i];
            flow[i][n+j] = B[i].capacity - flow[n+j][i];
            inflow[j] += flow[n+j][i];
        }
    }
    
    for (int j=0; j<m; j++){
        flow[n+j][sink] = C[j].capacity - inflow[j];
        flow[sink][n+j] = std::numeric_limits<int>::max();
    }
}

int bellmanFord(std::vector<vector <int>>& flow, std::vector<vector <int>>& cost, std::vector<int>& visited, std::vector<int>& distance, std::vector<int>& parent, int netSize, int source) {
    distance.assign(netSize, 0);
    visited.assign(netSize, 0);
    parent.assign(netSize, -1);
    queue <int> visitingQueue; // use the queue for optimization
    visitingQueue.push(source);
    while (!visitingQueue.empty()){
        int u = visitingQueue.front();
        visitingQueue.pop();
        for (int v = 0; v < netSize; v++) {
            if (flow[u][v] && (visited[v] == 0 || distance[u] + cost[u][v] < distance[v])) { // found new shortened visitedNegativeCycle
                visited[v]++;
                distance[v] = distance[u] + cost[u][v]; // new distance
                parent[v] = u; // new parent
                if (visited[v] > netSize) { // found negative cycle => proposed solution is not optimal
                    return v;
                }
                visitingQueue.push(v);
            }
        }
    }
    return -1; // No negative cycles found => solution is optimal
}


int main (void)
{
    int n; // Municipal buildings number
    int m; // Fallout shelters number
    vector <construction> B; // Buildings
    vector <construction> C; // Shelters
    
    int netSize; // Size of the network
    int current; // current vertex in a path (used in increased flow)
    int source; // Source
    int sink; // Sink
    // Min cost max flow algo
    vector <vector <int> > flow;
    vector <vector <int> > cost;
    
    vector <int> inflow; // Total number of workers evacuated to each shelter
    
    // bellmanFord
    vector <int> visited; // Vertex is visited
    vector <int> distance; // Distance to vertex
    vector <int> parent; // Parent of the vertex in the shortest visitedNegativeCycle
    
    vector <bool> visitedNegativeCycle;
    
    std::cin >> n >> m;
    
    netSize = n + m + 2;
    source = netSize - 2;
    sink = netSize - 1;
    
    B.resize(n);
    C.resize(m);
    
    flow.assign(netSize, vector <int> (netSize, 0));
    cost.assign(netSize, vector <int> (netSize, 0));
    inflow.assign(netSize, 0);
    
    for (int i = 0; i < n; i++) {
        std::cin >> B[i].x >> B[i].y >> B[i].capacity;
    }
    for (int i = 0; i < m; i++) {
        std::cin >> C[i].x >> C[i].y >> C[i].capacity;
    }
    
    initialize_network(flow, cost, inflow, n, m, B, C, source, sink);
    
    bool isOptimal = true;
    while ((current = bellmanFord(flow, cost, visited, distance, parent, netSize, source)) != -1) {
        isOptimal = false;
        int pathSource=current;
        visitedNegativeCycle.assign(netSize, false);
        while (!visitedNegativeCycle[pathSource]) {
            visitedNegativeCycle[pathSource] = true;
            pathSource = parent[pathSource];
        }
        
        current = pathSource;
        do { // Increase flow across the path from child to parent
            flow[current][parent[current]] += 1;
            flow[parent[current]][current] -= 1;
            current = parent[current];
        } while (current != pathSource);
    }
    
    if (isOptimal) {
        std::cout<<"OPTIMAL"<<std::endl;
    } else {
        std::cout<<"SUBOPTIMAL"<<std::endl;
        
        
        for (int i = 0; i < n; i++) {
            for (int j = n; j < n + m; j++) {
                std::cout<<flow[j][i]<<" ";
            }
            std::cout<<std::endl;
        }
    }
    
    return 0;
}
