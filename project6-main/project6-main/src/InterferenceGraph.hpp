#ifndef __PROJ6_INTERFERENCE_GRAPH__HPP
#define __PROJ6_INTERFERENCE_GRAPH__HPP

#include <stdexcept>
#include <string>
#include <unordered_set>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace shindler::ics46::project6 {

class UnknownVertexException : public std::runtime_error {
   public:
    explicit UnknownVertexException(const std::string &vertex)
        : std::runtime_error("Unkown vertex " + vertex) {}
};

class UnknownEdgeException : public std::runtime_error {
   public:
    UnknownEdgeException(const std::string &source,
                         const std::string &destination)
        : std::runtime_error("Unknown edge " + source + " - " + destination) {}
};

// InterferenceGraph
//
// This is a class representing an interference graph
// as described in "Part 1: Interference Graph" of the README.md
// file. Though this class is templated, because of the usage of exceptions
// UnknownVertexException and UnknownEdgeException it will
// ONLY be tested with strings.
template <typename T>
class InterferenceGraph {
   public:
    // Custom type used to represent edges. This is mainly
    // used in the utility function for reading and writing
    // the graph structure to/from files. You don't need to use it.
    using EdgeTy = std::pair<T, T>;

    InterferenceGraph();

    // You DO NOT need to implement these
    InterferenceGraph(const InterferenceGraph &) = delete;
    InterferenceGraph(InterferenceGraph &&) = default;
    InterferenceGraph &operator=(const InterferenceGraph &) = delete;
    InterferenceGraph &operator=(InterferenceGraph &&) = default;

    ~InterferenceGraph();



    void addEdge(const T &source, const T &destination);

    void addVertex(const T &vertex);

    void removeEdge(const T &source, const T &destination);

    void removeVertex(const T &vertex);

    [[nodiscard]] std::unordered_set<T> vertices() const noexcept;

    [[nodiscard]] std::unordered_set<T> neighbors(const T &vertex) const;

    [[nodiscard]] unsigned numVertices() const noexcept;

    [[nodiscard]] unsigned numEdges() const noexcept;

    [[nodiscard]] bool interferes(const T &source, const T &destination) const;

    [[nodiscard]] unsigned degree(const T &vertex) const;

   private:
   //Make a structure to store the data for each vertex

    struct vecData {
        vecData()
        : setNeighbors{}
        {}
        std::unordered_set<T> setNeighbors;
        bool operator<(vecData other) {
            return setNeighbors.size() < other.setNeighbors.size();
        }
    };
   //The primary data structure used to access vertice information
   std::unordered_map<T, vecData> AdjList;
   //Store the vertices in this set for vertices function
   std::unordered_set<T> verticeSet;
   //Keep count of the edges here
   unsigned edgeCount{0};
};

template <typename T>
InterferenceGraph<T>::InterferenceGraph() {
    
}

template <typename T>
InterferenceGraph<T>::~InterferenceGraph() {
    // TODO: Implement this
}


template <typename T>
std::unordered_set<T> InterferenceGraph<T>::neighbors(const T &vertex) const {
    if(AdjList.count(vertex) == 0)
    {
        throw UnknownVertexException("No Vertex Exist Remove Vertex");
    }
    return AdjList.at(vertex).setNeighbors;
}

template <typename T>
std::unordered_set<T> InterferenceGraph<T>::vertices() const noexcept {
    return verticeSet;
}

template <typename T>
unsigned InterferenceGraph<T>::numVertices() const noexcept {
    return AdjList.size();
}

template <typename T>
unsigned InterferenceGraph<T>::numEdges() const noexcept {
    return edgeCount;
}

template <typename T>
void InterferenceGraph<T>::addEdge(const T &source, const T &destination) {
    if (AdjList.count(source) == 0 or AdjList.count(destination) == 0)
    {
        throw UnknownVertexException("removeEdge: No Vertex or Destination");
    }
    //Add the new destination edge to the vector
    AdjList.at(source).setNeighbors.insert(destination);
    //Add the new source edge to the vector
    AdjList.at(destination).setNeighbors.insert(source);
    edgeCount++;
}

template <typename T>
void InterferenceGraph<T>::removeEdge(const T &source, const T &destination) {
    if (AdjList.count(source) == 0 or AdjList.count(destination) == 0)
    {
        throw UnknownVertexException("removeEdge: No Vertex or Destination");
    }
    //Remove the destination from the source list
    AdjList.at(source).setNeighbors.erase(destination);
    //Remove the source from the destination list
    AdjList.at(destination).setNeighbors.erase(source);
    edgeCount--;
}

template <typename T>
void InterferenceGraph<T>::addVertex(const T &vertex) {
    if (AdjList.count(vertex) == 0) {
        AdjList[vertex] = vecData();
        verticeSet.insert(vertex);
    }
}


template <typename T>
void InterferenceGraph<T>::removeVertex(const T &vertex) {
    std::cout << vertex;
    //Check if the vertex exist in the map. If it does not throw error
    if(AdjList.count(vertex) == 0)
    {
        throw UnknownVertexException("No Vertex Exist Remove Vertex");
    }

    //Iterate through the neighbors of the vertex and delete the vertex from those neighbors vectors
    for (auto val: AdjList.at(vertex).setNeighbors)
    {
        //Erase the vertex from its adjacent nodes
        AdjList.at(val).setNeighbors.erase(vertex);
        edgeCount--;
    }
    verticeSet.erase(vertex);
    AdjList.erase(vertex);
}

template <typename T>
bool InterferenceGraph<T>::interferes(const T &source,
                                      const T &destination) const {
    if (AdjList.count(source) == 0 or AdjList.count(destination) == 0)
    {
        throw UnknownVertexException("removeEdge: No Vertex or Destination");
    }
    if(AdjList.at(source).setNeighbors.count(destination) == 1){
        return true;
    }
    return false;
}

template <typename T>
unsigned InterferenceGraph<T>::degree(const T &vertex) const {
    if(AdjList.count(vertex) == 0)
    {
        throw UnknownVertexException("Degree: No Vertex Exist");
    }
    return AdjList.at(vertex).setNeighbors.size();
}
};  // namespace shindler::ics46::project6

#endif
