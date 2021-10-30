#pragma once

#ifndef YK_RAYTRACING_ISOTROPIC_HPP
#define YK_RAYTRACING_ISOTROPIC_HPP

#include "../color.hpp"
#include "../custom.hpp"
#include "../hit_record.hpp"
#include "../ray.hpp"
#include "../texture.hpp"
#include "../vec3.hpp"

namespace yk {

template <class T>
struct isotropic {
  texture<T> albedo;

  template <class Gen>
  constexpr bool scatter(const ray<T>& r_in, const hit_record<T>& rec,
                         color<T>& attenuation, ray<T>& scattered,
                         Gen& gen) const noexcept {
    scattered = ray<T>{rec.pos, random_in_unit_sphere<T>(gen), r_in.time};
    attenuation = custom::value(albedo, rec.u, rec.v, rec.pos);
    return true;
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_ISOTROPIC_HPP
