#pragma once

#ifndef YK_RAYTRACING_METAL_HPP
#define YK_RAYTRACING_METAL_HPP

#include "../color.hpp"
#include "../hit_record.hpp"
#include "../ray.hpp"
#include "../vec3.hpp"

namespace yk {

template <class T>
struct metal {
  color<T> albedo;

  template <class Gen>
  constexpr bool scatter(const ray<T>& r, const hit_record<T>& rec,
                         color<T>& attenuation, ray<T>& scattered,
                         Gen&) const noexcept {
    vec3<T> reflected = reflect(r.direction.normalized(), rec.normal);
    scattered = ray<T>{rec.pos, reflected};
    attenuation = albedo;
    return (dot(scattered.direction, rec.normal) > 0);
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_METAL_HPP
