#include <CSVReader.hpp>
#include <IGWriter.hpp>
#include <InterferenceGraph.hpp>
#include <catch2/catch_amalgamated.hpp>
#include <register_allocation.hpp>
#include <verifier.hpp>

namespace {

namespace proj6 = shindler::ics46::project6;

const std::string &GRAPH_DIR = std::string("tst/graphs/");

TEST_CASE("SimpleSuccess", "[Required][RA]") {
    // Load the graph pointed to by simple.csv
    const auto &GRAPH = GRAPH_DIR + "simple.csv";
    const auto NUM_REGS = 3;

    // Provide 3 registers to the register allocation algorithm.
    // Note, the highest degree in this graph is two, so all three
    // will be needed.
    const auto &allocation = proj6::assignRegisters(GRAPH, NUM_REGS);
    proj6::IGWriter::write(proj6::CSVReader::load(GRAPH),
                           GRAPH_DIR + "simple_success.dot", allocation);

    REQUIRE((verifyAllocation(GRAPH, NUM_REGS, allocation)));
}

TEST_CASE("SimpleFail", "[Required][RA]") {
    // Load the graph pointed to by simple.csv
    const auto &GRAPH = GRAPH_DIR + "simple.csv";
    const auto NUM_REGS = 2;

    // Provide 2 registers to the register allocation algorithm.
    // Note, the highest degree in this graph is two, so the allocation
    // cannot be completed.
    const auto &allocation = proj6::assignRegisters(GRAPH, NUM_REGS);
    REQUIRE((allocation.empty()));
}

TEST_CASE("Bipartie", "[Required][RA]") {
    // Load the graph pointed to by simple.csv
    const auto &GRAPH = GRAPH_DIR + "big_bipartite.csv";
    const auto NUM_REGS = 20;

    // Provide 2 registers to the register allocation algorithm.
    // Note, the highest degree in this graph is two, so the allocation
    // cannot be completed.
    const auto &allocation = proj6::assignRegisters(GRAPH, NUM_REGS);
    proj6::IGWriter::write(proj6::CSVReader::load(GRAPH),
                           GRAPH_DIR + "simple_success.dot", allocation);

    REQUIRE((verifyAllocation(GRAPH, NUM_REGS, allocation)));
}

TEST_CASE("complete_1k_registers", "[Extra][RA]") {
    // Load the graph pointed to by simple.csv
    const auto &GRAPH = GRAPH_DIR + "complete_1000.csv";
    const auto NUM_REGS = 1000;

    const auto &allocation = proj6::assignRegisters(GRAPH, NUM_REGS);
    proj6::IGWriter::write(proj6::CSVReader::load(GRAPH),
                           GRAPH_DIR + "simple_success.dot", allocation);

    REQUIRE((verifyAllocation(GRAPH, NUM_REGS, allocation)));
}

}  // namespace
