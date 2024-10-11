#ifndef __PROJ6_REGISTER_ALLOCATION__HPP
#define __PROJ6_REGISTER_ALLOCATION__HPP

#include <string>
#include <unordered_map>
#include <vector>
#include "InterferenceGraph.hpp"

namespace shindler::ics46::project6 {

using Variable = std::string;
using Register = int;
using RegisterAssignment = std::unordered_map<Variable, Register>;

RegisterAssignment assignRegisters(const std::string &pathToGraph,
                                   int numRegisters) noexcept;


//Code used from the zybooks activity
int partition(std::vector<Variable>& vertices, InterferenceGraph<Variable>& igraph, int lowIndex, int highIndex);
   

void quicksort(std::vector<Variable>& vertices, InterferenceGraph<Variable>& igraph, int lowIndex, int highIndex);

bool compareDegrees(const Variable& v1, const Variable& v2, const InterferenceGraph<Variable>& igraph);



};  // namespace shindler::ics46::project6

#endif
