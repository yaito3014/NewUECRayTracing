#pragma once

#ifndef YK_RAYTRACING_LAMBERTTIAN_HPP
#define YK_RAYTRACING_LAMBERTTIAN_HPP

#include <memory>
#include <utility>

#include "../color.hpp"
#include "../custom.hpp"
#include "../hit_record.hpp"
#include "../random.hpp"
#include "../ray.hpp"
#include "../texture.hpp"
#include "../textures/solid_color.hpp"

namespace yk {

template <class T>
struct lambertian {
  texture<T> albedo;

  template <class Gen>
  constexpr bool scatter(const ray<T>& r, const hit_record<T>& rec,
                         color<T>& attenuation, ray<T>& scattered,
                         Gen& gen) const noexcept {
    auto scatter_direction = rec.normal + random_unit_vector<T>(gen);
    if (scatter_direction.near_zero()) scatter_direction = rec.normal;
    scattered = ray<T>{rec.pos, scatter_direction, r.time};
    attenuation = custom::value(albedo, rec.u, rec.v, rec.pos);
    return true;
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_LAMBERTIAN_HPP
