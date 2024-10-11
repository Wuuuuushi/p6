#include "register_allocation.hpp"

#include "CSVReader.hpp"
#include "InterferenceGraph.hpp"
#include <iostream>

namespace shindler::ics46::project6 {

// assignRegisters
//
// This is where you implement the register allocation algorithm
// as mentioned in the README. Remember, you must allocate at MOST
// d(G) + 1 registers where d(G) is the maximum degree of the graph G.
// If num_registers is not enough registers to accomodate the passed in
// graph you should return an empty map. You MUST use registers in the
// range [1, num_registers] inclusive.
RegisterAssignment assignRegisters(const std::string &pathToGraph,
                                   int numRegisters) noexcept {
    InterferenceGraph<Variable> igraph = CSVReader::load(pathToGraph);
   //This is the amount of usedRegister. Starts a 1
    int usedRegister{1};

   //This is the return type
    RegisterAssignment assignments{};

    std::unordered_set<std::string> unUsedVertex{};
    unUsedVertex = igraph.vertices();

   std::vector<std::string> orderedVertex; //Used for sorting and iterating through for the color graph
    for (auto val: igraph.vertices())
    {
        orderedVertex.push_back(val);
    }

   //Quicksort algorithm sorts through the vector and makes it greatest to smallest
   quicksort(orderedVertex, igraph, 0, orderedVertex.size() - 1);
   //Checks if the biggest degree is smaller than the number of registers. If it is not return empty map
    if(igraph.degree(orderedVertex[0]) + 1 > static_cast<unsigned> (numRegisters))
    {
        return assignments;
    }

    //Keep track of orderedVertex 
    std::unordered_set<std::string> neighborSet{};

    for (auto val: orderedVertex)
    {
      if(unUsedVertex.size() == 0)
      {
         break;
      }
      if (unUsedVertex.count(val) == 1)
      {
         neighborSet = igraph.neighbors(val);
         for (auto vertex: igraph.vertices())
         {
            if (neighborSet.count(vertex) == 0  and unUsedVertex.count(vertex) == 1)
            {
               assignments[vertex] = usedRegister;
               unUsedVertex.erase(vertex);
            }
         }
         usedRegister++;
      }
    }
   return assignments;
    //For the first case we do all vertexs who are not neighbors to the biggest degree
   //  for (auto val: igraph.vertices())
   //  {
   //    if (neighborSet.count(val) == 0 and unUsedVertex.count(val) == 1)
   //    {
   //       assignments[val] = usedRegister;
   //       unUsedVertex.erase(val);
   //    }
   //  }
   //  usedRegister++;

   // for (auto neighbors: igraph.neighbors(orderedVertex[0]))
   //  {
   //    neighborSet = igraph.neighbors(neighbors);
   //    assignments[neighbors] = usedRegister;
   //    unUsedVertex.erase(neighbors);
   //    for (auto val: igraph.vertices())
   //    {
   //       if (neighborSet.count(val) == 0 and unUsedVertex.count(val) == 1)
   //       {
   //          assignments[val] = usedRegister;
   //          unUsedVertex.erase(val);
   //       }
      
   //    }
   //    usedRegister++;
   // }
   //  return assignments;
}

bool compareDegrees(const Variable& v1, const Variable& v2, const InterferenceGraph<Variable>& igraph) {
    return igraph.degree(v1) > igraph.degree(v2);
}


// Quicksort implementation (pass by reference)
void quicksort(std::vector<Variable>& vertices, InterferenceGraph<Variable>& igraph, int lowIndex, int highIndex) {
    if (lowIndex < highIndex) {
        int partitionIndex = partition(vertices, igraph, lowIndex, highIndex);
        quicksort(vertices, igraph, lowIndex, partitionIndex);
        quicksort(vertices, igraph, partitionIndex + 1, highIndex);
    }
}

// Partition function for quicksort
int partition(std::vector<Variable>& vertices, InterferenceGraph<Variable>& igraph, int lowIndex, int highIndex) {
    Variable pivot = vertices[(lowIndex + highIndex) / 2];
    int i = lowIndex - 1;
    int j = highIndex + 1;
    while (true) {
        do {
            i++;
        } while (compareDegrees(vertices[i], pivot, igraph));
        do {
            j--;
        } while (compareDegrees(pivot, vertices[j], igraph));
        if (i >= j) {
            return j;
        }
        std::swap(vertices[i], vertices[j]);
    }
}

}  // namespace shindler::ics46::project6
