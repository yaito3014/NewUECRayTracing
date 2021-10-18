#pragma once

#ifndef YK_RAYTRACING_DIELECTRIC_HPP
#define YK_RAYTRACING_DIELECTRIC_HPP

#include "../color.hpp"
#include "../hit_record.hpp"
#include "../ray.hpp"

namespace yk {

template <class T>
struct dielectric {
  T ir;  // index_of_refraction

  constexpr dielectric(T index_of_refraction) : ir(index_of_refraction) {}

  template <class Gen>
  constexpr bool scatter(const ray<T>& r_in, const hit_record<T>& rec,
                         color<T>& attenuation, ray<T>& scattered,
                         Gen& gen) const noexcept {
    constexpr auto reflectance = [](T cosine, T ref_idx) {
      // Use Schlick's approximation for reflectance.
      auto r0 = (1 - ref_idx) / (1 + ref_idx);
      r0 = r0 * r0;
      return r0 + (1 - r0) * math::pow((1 - cosine), 5);
    };

    attenuation = color<T>(1.0, 1.0, 1.0);
    T refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

    auto unit_direction = r_in.direction.normalized();

    T cos_theta = std::min<T>(dot(-unit_direction, rec.normal), 1);
    T sin_theta = math::sqrt(1 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    vec3<T> direction =
        cannot_refract || reflectance(cos_theta, refraction_ratio) >
                              uniform_real_distribution<T>(0, 1)(gen)
            ? reflect(unit_direction, rec.normal)
            : refract(unit_direction, rec.normal, refraction_ratio);

    scattered = ray<T>(rec.pos, direction, r_in.time);
    return true;
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_DIELECTRIC_HPP
