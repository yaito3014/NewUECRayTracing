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
#include "yk/custom.hpp"
#include "yk/hittables/hittable_list.hpp"
#include "yk/hittables/moving_sphere.hpp"
#include "yk/hittables/sphere.hpp"
#include "yk/materials/dielectric.hpp"
#include "yk/materials/lambertian.hpp"
#include "yk/materials/metal.hpp"
#include "yk/random.hpp"

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

template <class T, class Gen>
constexpr yk::hittable<T> get_world(Gen& gen) noexcept {
  yk::hittable_list<T> world;

  auto ground_material = yk::lambertian<T>({0.5, 0.5, 0.5});
  world.add(yk::sphere<T>{{0, -1000, 0}, 1000, ground_material});

  yk::uniform_real_distribution<T> dist(0, 1);

  for (int a = -11; a < 11; ++a) {
    for (int b = -11; b < 11; ++b) {
      auto choose_mat = dist(gen);
      yk::pos3<T> center(a + 0.8 * dist(gen), 0.2, b + 0.8 * dist(gen));
      if ((center - yk::pos3<T>(4, 0.2, 0)).length() <= 0.9) continue;

      if (choose_mat < 0.8) {
        // diffuse
        auto albedo = yk::color<T>::random(gen) * yk::color<T>::random(gen);
        auto center2 = center + yk::vec3<T>(0, dist(gen) / 2, 0);
        world.add(yk::moving_sphere<T>(center, center2, 0, 1, 0.2,
                                       yk::lambertian<T>(albedo)));
      } else if (choose_mat < 0.95) {
        // metal
        auto albedo = yk::color<T>::random(0.5, 1, gen);
        auto fuzz = dist(gen) / 2;
        world.add(yk::sphere<T>(center, 0.2, yk::metal<T>(albedo, fuzz)));
      } else {
        // glass
        world.add(yk::sphere<T>(center, 0.2, yk::dielectric<T>(1.5)));
      }
    }
  }

  world.add(yk::sphere<T>({0, 1, 0}, 1.0, yk::dielectric<T>(1.5)));

  world.add(yk::sphere<T>({-4, 1, 0}, 1.0, yk::lambertian<T>({0.4, 0.2, 0.1})));

  world.add(yk::sphere<T>({4, 1, 0}, 1.0, yk::metal<T>({0.7, 0.6, 0.5}, 0.0)));

  return world;
}

template <class T>
constexpr auto render() noexcept {
  auto R = cos(3.14159265358979 / 4);

  yk::mt19937 mt(std::random_device{}());
  yk::uniform_real_distribution<T> dist(0, 1);

  std::vector<yk::color<std::uint8_t>> img(yk::constants::image_width *
                                           yk::constants::image_height);

  /* std::array<yk::color<std::uint8_t>,
              yk::constants::image_width * yk::constants::image_height>
       img{};*/

  auto world = get_world<T>(mt);

  yk::pos3<T> lookfrom(13, 2, 3);
  yk::pos3<T> lookat(0, 0, 0);
  yk::vec3<T> vup(0, 1, 0);
  auto dist_to_focus = 10.0;
  auto aperture = 0.1;

  yk::camera<T> cam(lookfrom, lookat, vup, 20, yk::constants::aspect_ratio,
                    aperture, dist_to_focus, 0, 1);

  for (int h = 0; h < yk::constants::image_height; ++h) {
    if (not std::is_constant_evaluated()) std::cout << "line : " << h << '\n';
    for (int w = 0; w < yk::constants::image_width; ++w) {
      std::vector<yk::ray<T>> rays(yk::constants::samples_per_pixel);
      std::generate(rays.begin(), rays.end(), [&]() {
        auto u = double(w + dist(mt)) / yk::constants::image_width;
        auto v = double(yk::constants::image_height - h - dist(mt)) /
                 yk::constants::image_height;
        return cam.get_ray(u, v, mt);
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
