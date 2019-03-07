#ifndef MINCOSTFLOWSOLVER
#define MINCOSTFLOWSOLVER

namespace mincost {

// An edge is represented as a struct
// Fields:
//      destination   -  denotes the ending node of an edge. For example, 'v' in u-->v
//      capacity      -  the maximum capacity of an edge
//      residualFlow  -  the residual amount of flow that can flow through the edge
//      counterEdge   -  a pointer to the counter edge in residual graph for performance optimization
struct Edge{
    int destination;
    int capacity;
    int residualFlow;
    double cost;
    Edge* counterEdge;
};

// A graph is represented as a struct
// Fields:
//      numVertices - denotes the number of vertices in the graph
//      adj         - Adjacency list : Collection of unordered lists one for each vertex
struct Graph{
    int numVertices;
    std::vector<Edge*> *adj;
};

Edge* genEdge(int destination, int capacity, int residualFlow, double cost);

// Prints all the edges in the graph
// Output:
//      List of edges where each edge is represented by
//          u(start node)   v(end node)   flow   capacity
int printGraph(const Graph& g);

void printParams(const Graph& g, int parentVertex[], int distance[]);

//int BFCycleDetection(const Graph& resGraph, int source, int parentVertex[], Edge* parentEdge[]);

double calcMinCostFlow(const Graph& graph, const Graph& residual, int s, int t, int requiredFlow);

} // namespace mincost

#endif
