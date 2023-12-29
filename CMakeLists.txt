cmake_minimum_required(VERSION 3.15.0)
project(wizlightcpp VERSION 1.0.0)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Werror -Wall -std=c++20")
SET(CMAKE_CXX_FLAGS  "${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_AUTOMOC ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/thirdparty)

set(HEADERS
        include/mainwindow.h
        include/LightIntensityWidget.h
        include/LightIntesityWidgetWithSlider.h
)

add_executable(${PROJECT_NAME} 
    ${CMAKE_SOURCE_DIR}/src/wizcontrol.cpp
    ${CMAKE_SOURCE_DIR}/src/log.cpp
    ${CMAKE_SOURCE_DIR}/src/udp.cpp
    ${CMAKE_SOURCE_DIR}/src/bulb.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/mainwindow.cpp
    ${CMAKE_SOURCE_DIR}/src/LightIntensityWidget.cpp
    ${CMAKE_SOURCE_DIR}/src/LightIntesityWidgetWithSlider.cpp
        ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Jansson JSON library
find_package(Jansson REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(${PROJECT_NAME} ${Jansson_LIBRARIES} nlohmann_json::nlohmann_json Qt5::Widgets)
target_include_directories(${PROJECT_NAME} PUBLIC ${Jansson_INCLUDE_DIRS})


if (WIN32)
target_link_libraries(${PROJECT_NAME} ws2_32)
endif()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
