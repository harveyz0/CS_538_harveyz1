#include "Vector.hpp"

namespace potato {

template<typename T, typename C>
class Line
{
public:
  Vec3<T> start{ 0, 0, 0 };
  Vec3<T> end{ 1, 1, 1 };

  Vec3<C> color{ 128, 128, 128 };
};

};
