#pragma once

#ifndef YK_RAYTRACING_CAMERA_HPP
#define YK_RAYTRACING_CAMERA_HPP

#include "config.hpp"
#include "pos3.hpp"
#include "ray.hpp"
#include "vec3.hpp"

namespace yk {

template <class T>
struct camera {
  pos3<T> origin;
  pos3<T> lower_left;
  vec3<T> horizontal;
  vec3<T> vertical;

  constexpr camera()
      : origin{0, 0, 0},
        horizontal{constants::viewport_width, 0, 0},
        vertical{0, constants::viewport_height, 0} {
    lower_left = {origin - horizontal / 2 - vertical / 2 -
                  vec3<T>{0, 0, constants::focal_length}};
  }

  template <class U>
  constexpr ray<T> get_ray(U u, U v) const {
    return ray<T>{origin, lower_left + u * horizontal + v * vertical - origin};
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_CAMERA_HPP
