cmake_minimum_required(VERSION 3.10)
project(ToyC_Parser)  # 更新项目名

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件，包含所有源文件
add_executable(toyc_parser
    src/lexer.cpp
    src/parser.cpp
    src/main.cpp
)

# 设置包含目录
target_include_directories(toyc_parser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# 设置输出文件名（可选）
set_target_properties(toyc_parser PROPERTIES OUTPUT_NAME "toyc_parser")