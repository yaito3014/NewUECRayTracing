#pragma once

#ifndef YK_RAYTRACING_MOVING_SPHERE_HPP
#define YK_RAYTRACING_MOVING_SPHERE_HPP

#include "../hit_record.hpp"
#include "../material.hpp"
#include "../pos3.hpp"

namespace yk {

template <class T>
struct moving_sphere {
  pos3<T> center0, center1;
  T time0, time1;
  T radius;
  material<T> mat;

  constexpr bool hit(const ray<T>& r, T t_min, T t_max,
                     hit_record<T>& rec) const noexcept {
    vec3<T> oc = r.origin - center(r.time);
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
    vec3<T> outward_normal = (rec.pos - center(r.time)) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat;

    return true;
  }

  constexpr bool bounding_box(T time0, T time1,
                              aabb<T>& output_box) const noexcept {
    aabb<T> box0{
        center(time0) - vec3<T>{radius, radius, radius},
        center(time0) + vec3<T>{radius, radius, radius},
    };
    aabb<T> box1{
        center(time1) - vec3<T>{radius, radius, radius},
        center(time1) + vec3<T>{radius, radius, radius},
    };
    output_box = surrounding_box(box0, box1);
    return true;
  }

  constexpr pos3<T> center(T time) const noexcept {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_MOVING_SPHERE_HPP
