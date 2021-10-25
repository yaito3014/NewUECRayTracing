#pragma once

#ifndef YK_RAYTRACING_HIT_RECORD_HPP
#define YK_RAYTRACING_HIT_RECORD_HPP

#include "material.hpp"
#include "ray.hpp"
#include "vec3.hpp"

namespace yk {

template <class T>
struct hit_record {
  pos3<T> pos;
  vec3<T> normal;
  material<T> mat;
  T t;
  T u, v;
  bool front_face;

  constexpr void set_face_normal(const ray<T>& r,
                                 const vec3<T>& outward_normal) noexcept {
    front_face = dot(r.direction, outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_HIT_RECORD_HPP
