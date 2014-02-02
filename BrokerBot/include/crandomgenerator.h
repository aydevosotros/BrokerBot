#ifndef CRANDOMGENERATOR_H
#define CRANDOMGENERATOR_H

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>

namespace FireDoorEscaper {

///(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((
/// \brief Singleton class that generates random numbers on demand
///(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((
class CRandomGenerator : private boost::noncopyable {
public:
  typedef std::vector<double> TVecDoubles;

  inline static  CRandomGenerator& get();
  inline int     uniformRandomInt(int min, int max);
  inline double  uniformRandomDouble(double min, double max);
  void           uniformRandomDoubleVector(TVecDoubles& vec, unsigned n, double min, double max);

private:
  CRandomGenerator();

  boost::random::random_device m_generator; //< Random device source of entropy
};

///
/// INLINE METHODS
///

///////////////////////////////////////////////////////////////////
/// \brief Accessor to the only instance of the object (Singleton)
/// \return The only instance of the object
///////////////////////////////////////////////////////////////////
CRandomGenerator&
CRandomGenerator::get() {
  static CRandomGenerator rnd;
  return rnd;
}

///////////////////////////////////////////////////////////////////
/// \brief Generates a random integer in [min, max]
/// Generates a random integer uniformelly distributed between
/// min and max, including both of them.
/// \return A random integer in [min, max]
///////////////////////////////////////////////////////////////////
int
CRandomGenerator::uniformRandomInt(int min, int max) {
  boost::random::uniform_int_distribution<> dist(min, max);
  return dist(m_generator);
}

///////////////////////////////////////////////////////////////////
/// \brief Generates a random double in [min, max]
/// Generates a random double uniformelly distributed between
/// min and max, including both of them.
/// \return A random double in [min, max]
///////////////////////////////////////////////////////////////////
double
CRandomGenerator::uniformRandomDouble(double min, double max) {
  boost::random::uniform_real_distribution<> dist(min, max);
  return dist(m_generator);
}

// Define for easiness of use
#define CRNDGEN CRandomGenerator::get()

} // Namespace

#endif // CRANDOMGENERATOR_H
