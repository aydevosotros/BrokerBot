#include "crandomgenerator.h"

namespace FireDoorEscaper {

/////////////////////////////////////////////////////////////////////////
/// \brief Constructs a RandomGenerator
/////////////////////////////////////////////////////////////////////////
CRandomGenerator::CRandomGenerator() {
}

/////////////////////////////////////////////////////////////////////////
/// \brief Appends n random doubles at the end of a std::vector
/// Generates n random doubles, uniformly distributed in [min, max] and
/// appends them to the end of a given std::vector (vec).
/// \param vec  Vector where to insert the doubles
/// \param n    Number of doubles to append
/// \param min  Minimum value for the randomly generated doubles
/// \param max  Maximum value for the randomly generated doubles
/////////////////////////////////////////////////////////////////////////
void
CRandomGenerator::uniformRandomDoubleVector(TVecDoubles &vec, unsigned n, double min, double max) {
  for (unsigned i=0; i < n; i++) {
    vec.push_back( uniformRandomDouble(min, max) );
  }
}

} // Namespace
