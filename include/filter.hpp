#ifndef FILTER_HPP
#define FILTER_HPP

#include "types.hpp"

namespace ns_filter {
  const float DEFAULT_TOLERANCE = 0.0f;
}

void filter(Samples& samples, const float tolerance);

#endif // FILTER_HPP