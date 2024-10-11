/**
   verifier.cpp

   You do NOT need to modify this file as part of your project. This is simply
   a utility for verifying that your register allocation is correct for a given
   graph.

*/

#include "verifier.hpp"

#include <CSVReader.hpp>
#include <InterferenceGraph.hpp>
#include <algorithm>
#include <fstream>

namespace proj6 = shindler::ics46::project6;

bool verifyAllocation(const std::string &path_to_graph, int num_registers,
                      const proj6::RegisterAssignment &mapping) {
    std::string line;
    std::ifstream file_stream(path_to_graph);
    std::unordered_map<proj6::Variable, unsigned> degrees;
    std::unordered_set<proj6::Variable> variables;
    std::vector<std::pair<proj6::Variable, proj6::Variable>> interferences;

    while (std::getline(file_stream, line)) {
        const auto &row = proj6::CSVReader::readRow(line);
        for (const auto &var : row) {
            variables.insert(var);
            if (degrees.find(var) == degrees.end()) {
                degrees[var] = 0;
            }
        }
        if (row.size() == 2) {
            degrees[row[0]]++;
            degrees[row[1]]++;
            interferences.emplace_back(row[0], row[1]);
        }
    }

    for (const auto &vertex : variables) {
        if (mapping.find(vertex) == mapping.end()) {
            throw std::runtime_error(
                std::string("Variable ") + vertex +
                std::string(" did not get mapped to a register!"));
        }

        if (mapping.at(vertex) < 1 || mapping.at(vertex) > num_registers) {
            throw std::runtime_error(std::string("Variable ") + vertex +
                                     std::string(" mapped to register ") +
                                     std::to_string(mapping.at(vertex)) +
                                     std::string(" which is out of range [") +
                                     std::to_string(1) + std::string(",") +
                                     std::to_string(num_registers) +
                                     std::string("]"));
        }
    }

    for (const auto &interference : interferences) {
        if (mapping.at(interference.first) == mapping.at(interference.second)) {
            throw std::runtime_error(
                std::string("Variables ") + interference.first +
                std::string(" and ") + interference.second +
                std::string(" were mapped to the same register: ") +
                std::to_string(mapping.at(interference.first)));
        }
    }

    if (variables.empty()) {
        return true;
    }

    const auto highest_degree =
        std::max_element(std::begin(degrees), std::end(degrees),
                         [](const auto &pt1, const auto &pt2) {
                             return pt1.second < pt2.second;
                         })
            ->second;

    std::unordered_set<proj6::Register> unique_registers;
    for (const auto &map : mapping) {
        unique_registers.insert(map.second);
    }

    const auto MAX_ALLOWED_REGS = highest_degree + 1;
    const auto NUM_USED_REGS = unique_registers.size();
    if (NUM_USED_REGS > MAX_ALLOWED_REGS) {
        throw std::runtime_error("Too many registers were used!");
    }

    return true;
}
