#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <execution>
#include <functional>
#include <iostream>
#include <limits>
#include <random>

#include "yk/camera.hpp"
#include "yk/color.hpp"
#include "yk/hittables/hittable_list.hpp"
#include "yk/hittables/sphere.hpp"
#include "yk/materials/lambertian.hpp"
#include "yk/materials/metal.hpp"
#include "yk/random.hpp"
#include "yk/scatter.hpp"

template <class T, class Gen>
constexpr yk::color<T> ray_color(const yk::ray<T>& r,
                                 const yk::hittable<T>& world,
                                 unsigned int depth, Gen& gen) noexcept {
  if (depth == 0) return {0, 0, 0};
  yk::hit_record<T> rec{};
  if (yk::hit<T>(world, r, 0.001, std::numeric_limits<T>::infinity(), rec)) {
    yk::ray<T> scattered;
    yk::color<T> attenuation{0, 0, 0};
    if (yk::scatter<T>(rec.mat, r, rec, attenuation, scattered, gen))
      return attenuation * ray_color(scattered, world, depth - 1, gen);
    return {0, 0, 0};
  }
  auto t = 0.5 * (r.direction.normalized().y + 1.0);
  return (1.0 - t) * yk::color<T>{1, 1, 1} + t * yk::color<T>{0.5, 0.7, 1.0};
}

template <class T>
constexpr yk::color<std::uint8_t> into(const yk::color<T>& c) {
  return {
      static_cast<std::uint8_t>(256 *
                                std::clamp<T>(yk::math::sqrt(c.r), 0, 0.999)),
      static_cast<std::uint8_t>(256 *
                                std::clamp<T>(yk::math::sqrt(c.g), 0, 0.999)),
      static_cast<std::uint8_t>(256 *
                                std::clamp<T>(yk::math::sqrt(c.b), 0, 0.999)),
  };
}

template <class T>
constexpr yk::hittable<T> get_world() noexcept {
  yk::hittable_list<double> world;

  auto material_ground = yk::lambertian<T>(yk::color<T>(0.8, 0.8, 0.0));
  auto material_center = yk::lambertian<T>(yk::color<T>(0.7, 0.3, 0.3));
  auto material_left = yk::metal<T>(yk::color<T>(0.8, 0.8, 0.8));
  auto material_right = yk::metal<T>(yk::color<T>(0.8, 0.6, 0.2));

  world.add(
      yk::sphere<T>{yk::pos3<T>{0.0, -100.5, -1.0}, 100.0, material_ground});
  world.add(yk::sphere<T>(yk::pos3<T>(0.0, 0.0, -1.0), 0.5, material_center));
  world.add(yk::sphere<T>(yk::pos3<T>(-1.0, 0.0, -1.0), 0.5, material_left));
  world.add(yk::sphere<T>(yk::pos3<T>(1.0, 0.0, -1.0), 0.5, material_right));

  return world;
}

template <class T>
constexpr auto render() noexcept {
  yk::camera<T> cam;
  yk::mt19937 mt;
  yk::uniform_real_distribution<T> dist(0, 1);

  std::array<yk::color<std::uint8_t>,
             yk::constants::image_width* yk::constants::image_height>
      img = {};

  // std::vector<yk::color<std::uint8_t>> img(yk::constants::image_width *
  // yk::constants::image_height);

  auto world = get_world<T>();

  for (int h = 0; h < yk::constants::image_height; ++h) {
    std::cout << "line : " << h << '\n';
    for (int w = 0; w < yk::constants::image_width; ++w) {
      std::vector<yk::ray<T>> rays(yk::constants::samples_per_pixel);
      std::generate(rays.begin(), rays.end(), [&]() {
        auto u = double(w + dist(mt)) / yk::constants::image_width;
        auto v = double(yk::constants::image_height - h - dist(mt)) /
                 yk::constants::image_height;
        return cam.get_ray(u, v);
      });
      yk::color<T> pixel_color =
          std::transform_reduce(
              std::execution::par_unseq, rays.cbegin(), rays.cend(),
              yk::color<T>{0, 0, 0}, std::plus<>{},
              [&](const auto& ray) {
                return ray_color(ray, world, yk::constants::max_depth, mt);
              }) /
          yk::constants::samples_per_pixel;
      img[h * yk::constants::image_width + w] = into(pixel_color);
    }
  }

  return img;
}

int main() {
  auto img = render<double>();

  if (!stbi_write_png("image.png", yk::constants::image_width,
                      yk::constants::image_height, 3, img.data(),
                      sizeof(img[0]) * yk::constants::image_width))
    std::cerr << "error!" << std::endl;
}
