# CMakeList.txt: проект CMake для Corners; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

include_directories(${OPENCV_INCLUDE})

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Corners "Corners.cpp" "Corners.h")

target_link_libraries(Corners PRIVATE ${OPENCV_LIB}/opencv_world480d.lib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Corners PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
