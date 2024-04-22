
#include "interval.hpp"

namespace potato {
    const Interval Interval::empty    = Interval(+infinity, -infinity);
    const Interval Interval::universe = Interval(-infinity, +infinity);
}; // namespace potato
