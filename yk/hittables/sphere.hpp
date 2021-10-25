#pragma once

#ifndef YK_RAYTRACING_SPHERE_HPP
#define YK_RAYTRACING_SPHERE_HPP

#include <memory>

#include "../aabb.hpp"
#include "../hittable.hpp"
#include "../material.hpp"
#include "../pos3.hpp"
#include "../ray.hpp"

namespace yk {

template <class T>
struct sphere {
  pos3<T> center;
  T radius;
  material<T> mat;

  constexpr bool hit(const ray<T>& r, T t_min, T t_max,
                     hit_record<T>& rec) const noexcept {
    vec3<T> oc = r.origin - center;
    auto a = r.direction.length_squared();
    auto half_b = dot(oc, r.direction);
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = math::sqrt(discriminant);

    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
      root = (-half_b + sqrtd) / a;
      if (root < t_min || t_max < root) return false;
    }

    rec.t = root;
    rec.pos = r.at(rec.t);
    vec3<T> outward_normal = (rec.pos - center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat = mat;

    return true;
  }

  constexpr bool bounding_box(T time0, T time1,
                              aabb<T>& output_box) const noexcept {
    output_box = {
        center - vec3<T>{radius, radius, radius},
        center + vec3<T>{radius, radius, radius},
    };
    return true;
  }

  static constexpr void get_sphere_uv(const vec3<T>& p, T& u, T& v) noexcept {
    auto theta = math::acos(-p.y);
    auto phi = math::atan2(-p.z, p.x) + math::numbers::pi;
    u = phi / (2 * math::numbers::pi);
    v = theta / math::numbers::pi;
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_SPHERE_HPP
