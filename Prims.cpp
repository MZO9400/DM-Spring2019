#include <set>
#include <vector>
#include <iostream>

/*
** This program is a basic implementation of Prim's algorithm to find minimal
** spanning tree. Input is done through std::cin into a 2D array of integers
** then passed into a 2D vector. Sets are counted using std::vector's member
** function called size() and nodes (vertices) are formed from 0 to
** std::vector::size(), all of the nodes are put into unvisited set and they are
** removed from it and added into visited set one by one comparing weights of
** available edges and choosing the one with the minimum weight. The edge is then
** added to the tree which is then returned to main and is printed.
**
** Input is taken of only half the total size of the array because the other
** half is just the repeated edges, and any node's weight from itself is zero
*/


/*
** An edge contains a starting point, an ending point, and a weight associated
** to it. I've made a structure so it is possible to make a bundled vector array
*/
struct Edge {
    unsigned short int start;
    unsigned short int end;
    unsigned long long int weight;
};

/* An array of edges is same as a tree */
typedef std::vector<Edge> Tree;

/* Overloading of edge to std::ostream */
std::ostream& operator<<(std::ostream& _os, const Edge& ed) {
    _os << "Edge " << ed.start + 1 << " to " << ed.end + 1 << ", Weight: " <<
        ed.weight;
    return (_os);
}

/* This is the primary method of this program, this method takes a 2D Vector
** of integers and returns a tree
*/
Tree Prims_MST(const std::vector<std::vector<int>>&
                            graph) {
    /*
    ** STL <set> is a library used here which lets us save the vertices that are
    ** not visited yet. The library contains const data so editing is impossible
    ** once the data is added. However, ability to delete the data is there so I
    ** used that ability to save the unvisited nodes and then delete them as I
    ** visit the nodes one by one.
    */
    std::set<unsigned short int> unvisitedNodes;
    std::set<unsigned short int> visitedNodes;

    /*
    ** I've used std::vector here typedef'd as "Tree" (see Line17) because it is
    ** a standard template library and has certain advantages over normal
    ** arrays. The ability to use iterators and auto as well as faster insertion
    ** makes vectors better compared to cstyle arrays
    */
    Tree MST;

    // Starting point is always visitedNodes at no cost
    visitedNodes.insert(0);

    // Insert all nodes to unvisitedNodes
    for (auto i = 1;  i < graph.size(); ++i) {
        unvisitedNodes.insert(i);
    }

    while (!unvisitedNodes.empty()) {
        // Shortest edge is initialized with -1 as start and end, and INT_MAX as weight
        Edge shortestEdge = {-1, -1, (unsigned long long int)(-1)};

        // Put all edges (with their weights) from nodes that are in visitedNodes
        for (auto node : visitedNodes) {
            auto const& weightsInRows = graph[node];

            for (int end_node = 0;  end_node < weightsInRows.size();  ++end_node) {
                auto const weight = weightsInRows[end_node];

                if (weight > 0 && weight < shortestEdge.weight
                        && unvisitedNodes.count(end_node)) {

                    // New edge from node to end_node formed with weight: weight
                    shortestEdge = {node, (int)end_node, weight};
                }
            }
        }

        // Add the shortest path to the result
        MST.push_back(shortestEdge);

        // Mark the secondary node as visitedNodes
        unvisitedNodes.erase(shortestEdge.end);
        visitedNodes.insert(shortestEdge.end);
    }

    return MST;
}

std::vector<std::vector<int>> makeGraph() {
    unsigned short int NodesCount;
    std::cout << "Enter the number of vertices: ";
    std::cin >> NodesCount;
    int** GraphMatrix = new int*[NodesCount];

    for (int i = 0; i < NodesCount; ++i) {
        GraphMatrix[i] = new int[NodesCount];
    }

    for (int i = 0; i < NodesCount; i++) {
        for (int j = i + 1; j < NodesCount; j++) {
            std::cout << "Enter Row " << i + 1 << " Column " << j + 1
                      << "\'s value: ";
            std::cin >> GraphMatrix[i][j];
        }

        std::cout << std::endl << std::endl;
    }

    for (int i = 0; i < NodesCount; i++) {
        if (GraphMatrix[i][i]) {
            GraphMatrix[i][i] = 0; // Moving from Vertex N to N costs none
        }
    }

    for (int i = 0; i < NodesCount; i++) {
        for (int j = 0; j < NodesCount; j++) {
            if (GraphMatrix[i][j]) {
                GraphMatrix[j][i] = GraphMatrix[i][j]; // Cost from N-M == M-N
            }
        }
    }

    std::vector<int>* matrixRow = new std::vector<int>;
    std::vector<std::vector<int>> graph;

    for (int i = 0; i < NodesCount; i++) {
        for (int j = 0; j < NodesCount; j++) {
            matrixRow->push_back(GraphMatrix[i][j]);
        }
        graph.push_back(*matrixRow);
        matrixRow->clear();
    }

    delete matrixRow;

    for (int i = 0; i < NodesCount; i++) {
        delete[] GraphMatrix[i];
    }

    delete[] GraphMatrix;
    return graph;
}


int main() {
    for (auto const& iter : Prims_MST(makeGraph())) {
        std::cout << iter << '\n';
    }
}
