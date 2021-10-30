#pragma once

#ifndef YK_RAYTRACING_AARECT_HPP
#define YK_RAYTRACING_AARECT_HPP

#include <utility>

#include "../hit_record.hpp"
#include "../material.hpp"
#include "../ray.hpp"
#include "aabb.hpp"

namespace yk {

template <class T>
struct xy_rect {
  T x0, x1, y0, y1, k;
  material<T> mat;

  template <class Gen>
  constexpr bool hit(const ray<T>& r, T t_min, T t_max, hit_record<T>& rec,
                     Gen&) const noexcept {
    auto t = (k - r.origin.z) / r.direction.z;
    if (t < t_min || t > t_max) return false;
    auto x = r.origin.x + t * r.direction.x;
    auto y = r.origin.y + t * r.direction.y;
    if (x < x0 || x > x1 || y < y0 || y > y1) return false;
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    vec3<T> outward_normal = {0, 0, 1};
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat;
    rec.pos = r.at(t);
    return true;
  }

  constexpr bool bounding_box(T time0, T time1,
                              aabb<T>& output_box) const noexcept {
    output_box = {{x0, y0, k - 0.0001}, {x1, y1, k + 0.0001}};
    return true;
  }
};

template <class T>
struct xz_rect {
  T x0, x1, z0, z1, k;
  material<T> mat;

  template <class Gen>
  constexpr bool hit(const ray<T>& r, T t_min, T t_max, hit_record<T>& rec,
                     Gen& gen) const noexcept {
    auto t = (k - r.origin.y) / r.direction.y;
    if (t < t_min || t > t_max) return false;
    auto x = r.origin.x + t * r.direction.x;
    auto z = r.origin.z + t * r.direction.z;
    if (x < x0 || x > x1 || z < z0 || z > z1) return false;
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    vec3<T> outward_normal = {0, 1, 0};
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat;
    rec.pos = r.at(t);
    return true;
  }

  constexpr bool bounding_box(T time0, T time1,
                              aabb<T>& output_box) const noexcept {
    output_box = {{x0, k - 0.0001, z0}, {x1, k + 0.0001, z1}};
    return true;
  }
};

template <class T>
struct yz_rect {
  T y0, y1, z0, z1, k;
  material<T> mat;

  template <class Gen>
  constexpr bool hit(const ray<T>& r, T t_min, T t_max, hit_record<T>& rec,
                     Gen& gen) const noexcept {
    auto t = (k - r.origin.x) / r.direction.x;
    if (t < t_min || t > t_max) return false;
    auto y = r.origin.y + t * r.direction.y;
    auto z = r.origin.z + t * r.direction.z;
    if (y < y0 || y > y1 || z < z0 || z > z1) return false;
    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    vec3<T> outward_normal = {1, 0, 0};
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat;
    rec.pos = r.at(t);
    return true;
  }

  constexpr bool bounding_box(T time0, T time1,
                              aabb<T>& output_box) const noexcept {
    output_box = {{k - 0.0001, y0, z0}, {k + 0.0001, y1, z1}};
    return true;
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_AARECT_HPP
