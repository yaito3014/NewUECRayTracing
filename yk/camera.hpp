#pragma once

#ifndef YK_RAYTRACING_CAMERA_HPP
#define YK_RAYTRACING_CAMERA_HPP

#include <cmath>

#include "config.hpp"
#include "math.hpp"
#include "pos3.hpp"
#include "random.hpp"
#include "ray.hpp"
#include "vec3.hpp"

namespace yk {

template <class T>
struct camera {
  pos3<T> origin;
  pos3<T> lower_left;
  vec3<T> horizontal;
  vec3<T> vertical;
  vec3<T> w, u, v;
  T lens_radius;
  T time0, time1;

  constexpr camera(pos3<T> lookfrom, pos3<T> lookat, vec3<T> vup, T vfov,
                   T aspect_ratio, T aperture, T focus_dist, T t0,
                   T t1) noexcept
      : origin{lookfrom}, time0(t0), time1(t1) {
    auto theta = vfov * math::numbers::pi / 180;  // degree to radian
    auto h = tan(theta / 2);
    auto viewport_height = 2 * h;
    auto viewport_width = aspect_ratio * viewport_height;

    w = (lookfrom - lookat).normalized();
    u = cross(vup, w);
    v = cross(w, u);

    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    lower_left = {origin - horizontal / 2 - vertical / 2 - focus_dist * w};
    lens_radius = aperture / 2;
  }

  template <class U, class Gen>
  constexpr ray<T> get_ray(U s, U t, Gen& gen) const noexcept {
    vec3<T> rd = lens_radius * random_in_unit_disk<T>(gen);
    vec3<T> offset = u * rd.x + v * rd.y;
    return ray<T>{origin + offset,
                  lower_left + s * horizontal + t * vertical - origin - offset,
                  uniform_real_distribution<T>(0, 1)(gen)};
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_CAMERA_HPP
