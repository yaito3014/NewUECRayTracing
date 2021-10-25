#pragma once

#ifndef YK_RAYTRACING_AABB_HPP
#define YK_RAYTRACING_AABB_HPP

#include "pos3.hpp"
#include "ray.hpp"

namespace yk {

template <class T>
struct aabb {
  pos3<T> minimum;
  pos3<T> maximum;

  constexpr bool hit(const ray<T>& r, T t_min, T t_max) const noexcept {
    for (const auto& p : {&vec3<T>::x, &vec3<T>::y, &vec3<T>::z}) {
      auto [t0, t1] = std::minmax((minimum - r.origin).*p / r.direction.*p,
                                  (maximum - r.origin).*p / r.direction.*p);
      t_min = std::min(t_min, t0);
      t_max = std::max(t_max, t1);
      if (t_max <= t_min) return false;
    }
    return true;
  }
};

template <class T>
constexpr aabb<T> surrounding_box(aabb<T> box0, aabb<T> box1) noexcept {
  pos3<T> small{
      std::min(box0.minimum.x, box1.minimum.x),
      std::min(box0.minimum.y, box1.minimum.y),
      std::min(box0.minimum.z, box1.minimum.z),
  };
  pos3<T> big{
      std::max(box0.maximum.x, box1.maximum.x),
      std::max(box0.maximum.y, box1.maximum.y),
      std::max(box0.maximum.z, box1.maximum.z),
  };
  return aabb<T>{small, big};
}

}  // namespace yk

#endif  // !YK_RAYTRACING_AABB_HPP
