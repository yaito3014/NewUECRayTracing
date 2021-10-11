# CMakeList.txt : NewUECRayTracing の CMake プロジェクト。ソースを含めて、次を定義します:
# プロジェクト専用ロジックはこちらです。
#
cmake_minimum_required (VERSION 3.8)

project ("NewUECRayTracing")

# ソースをこのプロジェクトの実行可能ファイルに追加します。
add_executable (NewUECRayTracing "Source.cpp")

# TODO: テストを追加し、必要な場合は、ターゲットをインストールします。
