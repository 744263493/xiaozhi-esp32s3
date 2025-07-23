set(COMPONENT_SRCS "audio_utils.c" "another_source_file.c")  # 添加源文件
set(COMPONENT_ADD_INCLUDEDIRS "include" "libs")  # 添加头文件和库的路径

# 使用 ESP-IDF 提供的函数来设置库的依赖
set(COMPONENT_REQUIRES esp32)

# 引入 component.cmake 来执行构建
register_component()
