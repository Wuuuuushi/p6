#ifndef __PROJ6_VERIFIER__HPP
#define __PROJ6_VERIFIER__HPP

#include "register_allocation.hpp"

namespace proj6 = shindler::ics46::project6;

bool verifyAllocation(const std::string &path_to_graph, int num_registers,
                      const proj6::RegisterAssignment &mapping);

#endif
