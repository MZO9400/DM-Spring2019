#include <set>
#include <vector>
#include <iostream>
#include <algorithm>
#include <conio.h>
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

typedef unsigned short int uint2;
typedef unsigned long long int uint64;

/*
** An edge contains a starting point, an ending point, and a weight associated
** to it. I've made a structure so it is possible to make a bundled vector array
*/
struct Edge {
    uint2 start;
    uint2 end;
    uint64 weight;
    bool operator==(const Edge& _rhs) const;
    bool operator!=(const Edge& _rhs) const;
    bool operator<(const Edge& _rhs) const;
    bool operator>(const Edge& _rhs) const;
} dummyEdge{-1, -1, INT_MAX};

bool Edge::operator==(const Edge& _rhs) const {
    if (start == _rhs.start && end == _rhs.end && weight == _rhs.weight) {
        return true;
    }

    return false;
}
bool Edge::operator!=(const Edge& _rhs) const {
    if (*this == _rhs) {
        return false;
    }
    else {
        return true;
    }
}
bool Edge::operator<(const Edge& _rhs) const {
    if (weight < _rhs.weight) {
        return true;
    }
    else {
        return false;
    }
}
bool Edge::operator>(const Edge& _rhs) const {
    if (weight > _rhs.weight) {
        return true;
    }
    else {
        return false;
    }
}
/// ------------------------------------------------------------------------ \\\

/* An array of edges is same as a tree */
typedef std::vector<Edge> Tree;

/* Overloading of edge to std::ostream */
std::ostream& operator<<(std::ostream& _os, const Edge& ed) {
    _os << "Edge " << ed.start + 1 << " to " << ed.end + 1 << ", Weight: " <<
        ed.weight;
    return (_os);
}

/* Root is used to find whether two vertices have the same root i.e. causing loop */
uint2 getRoot(uint2& node, std::vector<uint2>& rootList) {
    while (rootList[node] != node) {
        node = rootList[node];
    }
    return node;
}


/* Better overload of the function for whole edge */
std::pair<uint2, uint2> getRoot(Edge _edge, std::vector<uint2>& rootList) {
    _edge.start = getRoot(_edge.start, rootList);
    _edge.end = getRoot(_edge.end, rootList);

    return std::pair<uint2, uint2>(_edge.start, _edge.end);
}


/*
** This is the primary method of this program, this method takes a 2D Vector
** of integers and returns a tree and its maximum weight.
*/
std::pair<Tree, uint64> Kruskal_MST(const std::vector<std::vector<int>>&
                                    graph) {
    Tree MST, graphTree;
    //          ^ Graph Tree is a tree which contains all possible edges
    Edge temp = dummyEdge;

    // Add all edges into the graphTree so that we can pop and check each
    for (int i = 0; i < graph.size(); i++) {
        for (int j = 0; j < graph.size(); j++) {

            bool found = 0;

            if (graph[i][j] != INT_MAX) {
                if (j > i) {
                    temp = {i, j, graph[i][j]};
                }
                else {
                    temp = {j, i, graph[i][j]};
                }
            }

            for (auto iter : graphTree) {
                if (temp == iter) {
                    found = 1;
                    break;
                }
            }

            if (!found && temp.weight != INT_MAX && temp.weight != 0) {
                graphTree.push_back(temp);
            }
        }
    }
    // Sort the container in descending order for pushing and popping smallest elem
    std::sort(graphTree.rbegin(), graphTree.rend());

    std::pair<uint2, uint2> parent;

    // A root list contains parent of each node
    std::vector<uint2> rootList(graph.size());

    // Initially, the parent of the node is the node itself
    for (int i = 0; i < rootList.size(); i++) {
        rootList[i] = i;
    }

    // Keep popping an edge off and checking if it is worth adding into MST
    while (!graphTree.empty()) {
        temp  = graphTree.back();
        graphTree.pop_back();
        parent = getRoot(temp, rootList);

        // If both the nodes have no common parent, add the edge to MST
        if (parent.first != parent.second) {
            MST.push_back(temp);

            // Set the node of second parent same as the first and vice versa
            if (parent.first < parent.second) {
                rootList[parent.second] = parent.first;
            }
            else {
                rootList[parent.first] = parent.second;
            }
        }
    }

    uint64 MSTWeight = 0;
    // Count total weight
    for (const auto& iter : MST) {
        MSTWeight += iter.weight;
    }

    return std::pair<Tree, uint64>(MST, MSTWeight);
}

/*
** This is the primary method of this program, this method takes a 2D Vector
** of integers and returns a tree and its maximum weight.
*/
std::pair<Tree, uint64> Prims_MST(const std::vector<std::vector<int>>&
                                  graph) {
    /*
    ** STL <set> is a library used here which lets us save the vertices that are
    ** not visited yet. The library contains const data so editing is impossible
    ** once the data is added. However, ability to delete the data is there so we
    ** used that ability to save the unvisited nodes and then delete them as we
    ** visit the nodes one by one.
    */
    std::set<uint2> unvisitedNodes;
    std::set<uint2> visitedNodes;

    /*
    ** we have used std::vector here typedef'd as "Tree" (see Line17) because it is
    ** a standard template library and has certain advantages over normal
    ** arrays. The ability to use iterators and auto as well as faster insertion
    ** makes vectors better compared to cstyle arrays
    */
    Tree MST;

    // Starting point is always visitedNodes with no cost
    visitedNodes.insert(0);

    // Insert all nodes to unvisitedNodes
    for (auto i = 1;  i < graph.size(); ++i) {
        unvisitedNodes.insert(i);
    }

    while (!unvisitedNodes.empty()) {
        // Shortest edge is initialized with -1 as start and end, and INT_MAX as weight
        Edge shortestEdge = dummyEdge;

        // Put all edges (with their weights) from nodes that are in visitedNodes
        for (auto node : visitedNodes) {

            for (int end_node = 0;  end_node < graph[node].size(); ++end_node) {
                auto const weight = graph[node][end_node];

                if (weight > 0 && weight < shortestEdge.weight
                        && unvisitedNodes.count(end_node)) {

                    // New edge from node to end_node formed with weight: weight
                    shortestEdge = {node, end_node, weight};
                }
            }
        }

        // Add the shortest path to the result
        MST.push_back(shortestEdge);

        // Mark the secondary node as visitedNodes
        unvisitedNodes.erase(shortestEdge.end);
        visitedNodes.insert(shortestEdge.end);
    }

    uint64 MSTWeight = 0;

    for (const auto& iter : MST) {
        MSTWeight += iter.weight;
    }

    return std::pair<Tree, uint64>(MST, MSTWeight);
}

std::vector<std::vector<int>> makeGraph() {
    uint2 NodesCount;
    std::cout << "Enter the number of vertices: ";
    std::cin >> NodesCount;
    int** GraphMatrix = new int*[NodesCount];

    for (int i = 0; i < NodesCount; ++i) {
        GraphMatrix[i] = new int[NodesCount];
    }

    system("cls");

    for (int i = 0; i < NodesCount; i++) {
        std::cout << "Enter 0 if there is no edge between the vertices\n\n\n";

        for (int j = i + 1; j < NodesCount; j++) {
            std::cout << "Enter Row " << i + 1 << " Column " << j + 1
                      << "\'s value: ";
            std::cin >> GraphMatrix[i][j];

            if (!GraphMatrix[i][j]) {
                GraphMatrix[i][j] = INT_MAX;
            }
        }

        system("cls");
    }

    for (int i = 0; i < NodesCount; i++) {
        if (GraphMatrix[i][i]) {
            GraphMatrix[i][i] = INT_MAX; // Moving from Vertex N to N costs none
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
    while (1) {
        system("cls");
        std::cout << "1) Calculate MST from Kruskal\n"
                  << "2) Calculate MST from Prims\n"
                  << "X) Exit\n\n\n";
        char choice;
        std::pair<Tree, uint64> pair;
        choice = _getch();

        if (choice == '1') {
            pair = Kruskal_MST(makeGraph());
            std::cout << "Calculated! The weight is: " << pair.second << "\n\n";
            system("pause");
        }
        else if (choice == '2') {
            pair = Prims_MST(makeGraph());
            std::cout << "Calculated! The weight is: " << pair.second << "\n\n";
            system("pause");
        }
        else if (choice == 'x' || choice == 'X') {
            system("cls");
            exit(0);
        }

        system("cls");

        for (auto const& iter : pair.first) {
            std::cout << iter << '\n';
        }

        std::cout << "\n" << "The weight of minimum spanning tree is: "
                  << pair.second << "\n\n";
        system("pause");

    }
}
