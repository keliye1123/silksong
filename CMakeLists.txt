cmake_minimum_required(VERSION 4.0)
project(hk_game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(./easyX/include)
link_directories(./easyX/lib64)

add_executable(hk_game main.cpp file.cpp file.h menu.cpp scene_1.cpp script.cpp boss_script.cpp boss_script.h function.cpp function.h UI.cpp UI.h)

target_link_libraries(hk_game
        ${CMAKE_SOURCE_DIR}/easyX/lib64/libeasyx.a  # 显式指定easyX静态库路径
        winmm                                       # Windows多媒体库（必须加）
        gdi32                                       # easyX依赖的GDI库（兼容兜底）
)