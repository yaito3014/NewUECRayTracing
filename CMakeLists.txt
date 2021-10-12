# CMakeList.txt : NewUECRayTracing の CMake プロジェクト。ソースを含めて、次を定義します:
# プロジェクト専用ロジックはこちらです。
#
cmake_minimum_required (VERSION 3.8)

project ("NewUECRayTracing")

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STACK_SIZE 10000000000)

# ソースをこのプロジェクトの実行可能ファイルに追加します。
add_executable (NewUECRayTracing "Source.cpp"   "yk/vec3.hpp" "yk/math.hpp" "yk/pos3.hpp" "yk/color.hpp" "yk/ray.hpp" "yk/camera.hpp" "yk/hittable.hpp" "yk/hittables/sphere.hpp" "yk/hittables/hittable_list.hpp" "yk/config.hpp" "yk/material.hpp" "yk/materials/lambertian.hpp" "yk/random.hpp" "yk/materials/metal.hpp" "yk/scatter.hpp" "yk/hit.hpp" "yk/hit_record.hpp")

find_path(STB_INCLUDE_DIRS "stb.h")
target_include_directories(NewUECRayTracing PRIVATE ${STB_INCLUDE_DIRS})

# TODO: テストを追加し、必要な場合は、ターゲットをインストールします。
