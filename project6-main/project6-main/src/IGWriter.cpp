/*0*
   IGWriter.cpp

   You do NOT need to modify this file as part of your project. See IGWriter.hpp
   for more info on how to use the graph writer. You may want to change the
   colors that are used for the different registers though :)

*/

#include "IGWriter.hpp"

#include <array>
#include <fstream>
#include <set>

#include "register_allocation.hpp"

namespace {

namespace proj6 = shindler::ics46::project6;

std::string lookupColor(const proj6::Variable &var,
                        const proj6::RegisterAssignment &regAssignment) {
    const int NUM_DEFAULT_COLORS = 8;
    // For alternate colors see: https://graphviz.org/doc/info/colors.html
    std::array<std::string, NUM_DEFAULT_COLORS> const colors = {
        "lightpink",       "lightsalmon",  "lightseagreen",   "lightskyblue4",
        "lightsteelblue1", "lightyellow1", "lightgoldenrod4", "lightcoral"};
    if (regAssignment.find(var) == regAssignment.end()) {
        // Value didn't get colored
        return "white";
    }

    int const reg = regAssignment.at(var);

    // Lookup color for this register
    if (reg <= NUM_DEFAULT_COLORS && reg >= 1) {
        return colors.at(reg - 1);
    }

    // More registers than colors. You get a nice boring grey :(
    return "darkgrey";
}

void writeEdges(const proj6::InterferenceGraph<proj6::Variable> &igraph,
                std::ofstream &stream) {
    std::set<proj6::InterferenceGraph<proj6::Variable>::EdgeTy> written_edges;

    for (const auto &source : igraph.vertices()) {
        for (const auto &destination : igraph.neighbors(source)) {
            if (written_edges.find(
                    proj6::InterferenceGraph<proj6::Variable>::EdgeTy(
                        source, destination)) != written_edges.end() ||
                written_edges.find(
                    proj6::InterferenceGraph<proj6::Variable>::EdgeTy(
                        destination, source)) != written_edges.end()) {
                continue;
            }

            stream << source << " -- " << destination << '\n';
            written_edges.insert(
                proj6::InterferenceGraph<proj6::Variable>::EdgeTy(source,
                                                                  destination));
        }
    }

    std::flush(stream);
}

void writeNodes(const proj6::InterferenceGraph<proj6::Variable> &igraph,
                std::ofstream &stream,
                const proj6::RegisterAssignment &register_assignment) {
    for (const auto &vertex : igraph.vertices()) {
        stream << vertex;
        stream << " [style=\"filled\", fillcolor="
               << lookupColor(vertex, register_assignment) << "]" << '\n';
    }
    std::flush(stream);
}

void writeIG(const proj6::InterferenceGraph<proj6::Variable> &igraph,
             std::ofstream &stream,
             const proj6::RegisterAssignment &register_assignment) {
    writeNodes(igraph, stream, register_assignment);
    writeEdges(igraph, stream);
}

};  // namespace

namespace shindler::ics46::project6 {

void IGWriter::write(const InterferenceGraph<Variable> &igraph,
                     const std::string &path,
                     const RegisterAssignment &registerAssignment) {
    std::ofstream fstream;
    fstream.open(path);
    fstream << "graph {" << '\n';
    fstream << "graph [layout=circo]" << '\n';
    writeIG(igraph, fstream, registerAssignment);
    fstream << "}";
    fstream.flush();
}

}  // namespace shindler::ics46::project6
