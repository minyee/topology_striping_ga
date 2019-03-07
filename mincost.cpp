#include <iostream>
#include <list>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <string>
#include <sstream>
#include <queue>
#include "mincost.h"


double DOUBLE_LARGE = 1000000000000.0;



// Graph g;
// Graph resGraph;


namespace mincost {
// Generates a new edge (allocating space dynamically) and returns a pointed to the edge
Edge* genEdge(int destination, int capacity, int residualFlow, double cost) {
    Edge* e1 = new Edge;
    e1->destination = destination;
    e1->capacity = capacity;
    e1->residualFlow = residualFlow;
    e1->cost = cost;
    return e1;
}

// Prints all the edges in the graph
// Output:
//      List of edges where each edge is represented by
//          u(start node)   v(end node)   flow   capacity
int printGraph(const Graph& g){
    for(int i=0; i<g.numVertices; i++){
        for(int j=0; j<g.adj[i].size(); j++){
            std::cout << i+1 << " " << g.adj[i][j]->destination+1 << " " << (g.adj[i][j]->capacity - g.adj[i][j]->residualFlow) << " " << g.adj[i][j]->cost << std::endl;
        }
    }
    return 0;
}

void printParams(const Graph& g, int parentVertex[], int distance[]){
    std::cout << "Parents Vertex:\n";
    for(int i=0; i<g.numVertices; i++){
        std::cout << parentVertex[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Distance Vertex:\n";
    for(int i=0; i< g.numVertices; i++){
        std::cout << distance[i] << " ";
    }
    std::cout << std::endl;
};

// Detects the presence of negative cycles in graph
// Output:
//      -1          if no negative cycles present
//      node_num    index of a node in negative cycle
// Lists parentVertex and parentEdge are updated and can be used to reconstruct the negative cycle
int BFCycleDetection(const Graph& resGraph, int source, int parentVertex[], Edge* parentEdge[]) {
    // Initialize variables that will be needed
    int cycle_node = -1;
    int numVertices = resGraph.numVertices;
    std::vector<Edge*> *adj = resGraph.adj;
    int distance[numVertices];
    // Initialize visited, parentVertex and distance
    for(int i=0; i<numVertices; i++){
        parentVertex[i] = -1;
        distance[i] = INT_MAX;
    }
    // BF - Relax edges repeatedly
    distance[source] = 0;
    for(int i=0; i<numVertices-1; i++){
        // loop on all edges
        for(int u=0; u<numVertices; u++){
            for(int e=0; e<adj[u].size(); e++){
                if(adj[u][e]->residualFlow > 0){
                    int v = adj[u][e]->destination;
                    int w = adj[u][e]->cost;
                    if(distance[v]>distance[u]+w){
                        distance[v] = distance[u]+w;
                        parentVertex[v] = u;
                        parentEdge[v] = adj[u][e];
                    }
                }
            }
        }
    }
    // Check for negative weight cycle - loop on all edges
    for(int u=0; u<numVertices; u++){
        for(int e=0; e<adj[u].size(); e++){
            if(adj[u][e]->residualFlow > 0){
                int v = adj[u][e]->destination;
                int w = adj[u][e]->cost;
                if(distance[v]>distance[u]+w){
                    return v;       // Negative cycle detected!
                }
            }
        }
    }
    return cycle_node;
}

// Cancels all negative cycles
// Output:
//      costSaved       amount of cost saved by cycle detection and cancellation
double cancelNegativeCycles(const Graph& g, const Graph& resGraph){
    double costSaved=0;
    int cyclePossible=1, u, v;
    Edge *te1, *te2;
    int numVertices = resGraph.numVertices;
    while(cyclePossible){
        cyclePossible=0;
        for(int i=0; i<numVertices; i++){
            int parent[g.numVertices];
            Edge* parentEdge[g.numVertices];
            int node_num = BFCycleDetection(resGraph, i, parent, parentEdge);
            if(node_num!=-1){               // A cycle is detected
                cyclePossible=1;
                // Calculate path flow
                int path_flow = INT_MAX;
                v=node_num; u = parent[v]; te1 = parentEdge[v];
                path_flow = std::min(path_flow, te1->residualFlow);
                for (v=u; v!=node_num; v=parent[v]){
                    u = parent[v];
                    te1 = parentEdge[v];
                    path_flow = std::min(path_flow, te1->residualFlow);
                }
                // Update graph by removing the cycle
                v=node_num; u = parent[v];
                te1 = parentEdge[v];
                te2 = te1->counterEdge;
                te1->residualFlow -= path_flow;
                te2->residualFlow += path_flow;
                costSaved += path_flow*(te1->cost);
                for (v=u; v != node_num; v=parent[v]){
                    u = parent[v];
                    te1 = parentEdge[v];
                    te2 = te1->counterEdge;
                    te1->residualFlow -= path_flow;
                    te2->residualFlow += path_flow;
                    costSaved += path_flow*(te1->cost);
                }
            }
        }
    }
    return -1*costSaved;
}


// Finds the shortest path from source to sink
// Output:
//      0           if no path exists from source to sink
//      1           if there is a path from source to sink
// Lists parentVertex and parentEdge are updated and can be used to reconstruct the shortest path
bool BF(Graph resGraph, int source, int sink, int parentVertex[], Edge* parentEdge[]){
    // Initialize variables that will be needed
    int numVertices = resGraph.numVertices;
    std::vector<Edge*> *adj = resGraph.adj;
    int distance[numVertices];
    // Initialize visited, parentVertex and distance
    for(int i=0; i<numVertices; i++){
        parentVertex[i] = -1;
        distance[i] = INT_MAX;
    }
    // printParams(parentVertex, distance);
    // BF
    distance[source] = 0;
    for(int i=0; i<numVertices-1; i++){
        // loop on all edges
        for(int u=0; u<numVertices; u++){
            for(int e=0; e<adj[u].size(); e++){
                if(adj[u][e]->residualFlow > 0){
                    int v = adj[u][e]->destination;
                    int w = adj[u][e]->cost;
                    if(distance[v]>distance[u]+w){
                        distance[v] = distance[u]+w;
                        parentVertex[v] = u;
                        parentEdge[v] = adj[u][e];
                    }
                }
            }
        }
        // printParams(parentVertex, distance);
    }
    if(parentVertex[sink] == -1){
        return false;
    }else{
        return true;
    }
}

// Calculates the cost of flow 'requiredFlow' from 's' to 't'
// Returns 'INT_MAX' if such a flow is not possible
double calcMinCostFlow(const Graph& graph, const Graph& residual, int s, int t, int requiredFlow) {
    int u, v, currFlow=0;
    double runningCost=0;
    Edge *te1, *te2;
    // Detect negative cycles and remove
    int benifit = cancelNegativeCycles(graph, residual);
    if(benifit){
        std::cout << "Negative cycle detected and removed. Resulting cost benifit is " << benifit << std::endl;
    }
    // Run shortest path augmentation
    int parent[graph.numVertices];
    Edge* parentEdge[graph.numVertices];
    while (BF(residual, s, t, parent, parentEdge)){
        int path_flow = INT_MAX;
        for (v=t; v!=s; v=parent[v]){
            u = parent[v];
            te1 = parentEdge[v];
            path_flow = std::min(path_flow, te1->residualFlow);
        }
        path_flow = std::min(path_flow, requiredFlow-currFlow);
        for (v=t; v != s; v=parent[v]){
            u = parent[v];
            te1 = parentEdge[v];
            te2 = te1->counterEdge;
            te1->residualFlow -= path_flow;
            te2->residualFlow += path_flow;
            runningCost += path_flow*(te1->cost);
        }
        currFlow += path_flow;
        if(currFlow == requiredFlow){
            break;
        }
    }
    if(currFlow == requiredFlow){
        return runningCost;
    }else{
        return DOUBLE_LARGE;
    }
}

}  // namespace mincost

/*
int main(){
    int numVertices, numEdges, source, destination, flow;
    int tu, tv, tcap, tcost, tflow;

    // Scan the graph and question configurations
    cin >> numVertices >> numEdges;
    cin >> flow;
    cin >> source >> destination;
    source--; destination--;                // as indexing starts from 0

    // Initialize the graphs
    g.numVertices = numVertices;
    g.adj = new vector<Edge*>[numVertices];
    resGraph.numVertices = numVertices;
    resGraph.adj = new vector<Edge*>[numVertices];

    // Input graph parameters
    for(int i=0; i<numEdges; i++){
        cin >> tu >> tv >> tcap >> tcost >> tflow;
        tu--; tv--;                         // as indexing starts from 0
        // Add edge to graph and edge and counter-edge to residual graph
        Edge* tmpEdge1 = genEdge(tv, tcap, tcap-tflow, tcost);
        Edge* tmpEdge2 = genEdge(tu, tcap, tflow, -1*tcost);
        tmpEdge1->counterEdge = tmpEdge2;
        tmpEdge2->counterEdge = tmpEdge1;
        g.adj[tu].push_back(tmpEdge1);
        resGraph.adj[tu].push_back(tmpEdge1);
        resGraph.adj[tv].push_back(tmpEdge2);
    }

    // Print initial state of graph
    cout << "Initial Graph:\n";
    printGraph(g);
    //cout << "hello\n";

    // Run min-cost flow algorithm
    double mincost = calcMinCostFlow(source, destination, flow);

    // Print final state of graph
    if(mincost == DOUBLE_LARGE){
        cout << "The given flow is not possible!\n";
    }else{
        cout << "The minimum cost for the additional flow is " << mincost << endl;
        cout << "Final Graph:\n";
        printGraph(g);
    }
    return 0;
}
*/
