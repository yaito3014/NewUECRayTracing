#pragma once

#ifndef YK_RAYTRACING_METAL_HPP
#define YK_RAYTRACING_METAL_HPP

#include "../color.hpp"
#include "../hit_record.hpp"
#include "../random.hpp"
#include "../ray.hpp"
#include "../vec3.hpp"

namespace yk {

template <class T>
struct metal {
  color<T> albedo;
  T fuzz;

  constexpr metal(const color<T>& a, T f)
      : albedo(a), fuzz(std::min<T>(f, 1)) {}

  template <class Gen>
  constexpr bool scatter(const ray<T>& r, const hit_record<T>& rec,
                         color<T>& attenuation, ray<T>& scattered,
                         Gen& gen) const noexcept {
    vec3<T> reflected = reflect(r.direction.normalized(), rec.normal);
    scattered = ray<T>{
        rec.pos, reflected + fuzz * random_in_unit_sphere<T>(gen), r.time};
    attenuation = albedo;
    return (dot(scattered.direction, rec.normal) > 0);
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_METAL_HPP
