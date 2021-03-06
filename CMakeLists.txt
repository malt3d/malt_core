cmake_minimum_required(VERSION 3.2)
project(malt)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../malt_cmake/")
set(CMAKE_CXX_STANDARD 14)

include(Malt)
include(CMakeGet)

find_package(yaml-cpp REQUIRED)
find_package(glm REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -undefined dynamic_lookup")
endif()

if(MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -Werror")
endif()

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/track_ptr/track_ptr.hpp" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/include/malt/")

set(HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/malt")
set(HEADER_FILES
        ${HEADER_PATH}/component.hpp
        ${HEADER_PATH}/entity.hpp
        ${HEADER_PATH}/module.hpp
        ${HEADER_PATH}/malt_fwd.hpp
        ${HEADER_PATH}/meta.hpp
        ${HEADER_PATH}/engine.hpp
        ${HEADER_PATH}/message.hpp
        ${HEADER_PATH}/utilities.hpp
        ${HEADER_PATH}/detail/game_impl.hpp
        ${HEADER_PATH}/track_ptr.hpp
        ${HEADER_PATH}/strided_iterator.hpp
        ${HEADER_PATH}/malt_reflect.hpp
        ${HEADER_PATH}/serialization.hpp
        ${HEADER_PATH}/debug.hpp
        ${HEADER_PATH}/error.hpp
        ${HEADER_PATH}/component_traits.hpp

        ${HEADER_PATH}/detail/malt_reflect_impl.hpp
        ${HEADER_PATH}/detail/entity_info.hpp
        ${HEADER_PATH}/detail/component_mgr_impl.hpp
        ${HEADER_PATH}/detail/component_mgr.hpp
        ${HEADER_PATH}/detail/game.hpp
        ${HEADER_PATH}/detail/game_impl.hpp
        )

set(SOURCE_FILES src/component.cpp src/entity.cpp src/utilities.cpp src/engine.cpp)

add_library(malt_core ${MALT_LIB_TYPE} ${SOURCE_FILES} ${HEADER_FILES})
malt_def_module(malt_core)

set_target_properties(malt_core PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(malt_core PROPERTIES C_VISIBILITY_PRESET hidden)

target_link_libraries(malt_core PUBLIC ${YAML_CPP_LIBRARIES})
target_include_directories(malt_core PUBLIC ${YAML_CPP_INCLUDE_DIR})
target_include_directories(malt_core PUBLIC ${GLM_INCLUDE_DIRS})

malt_install(malt_core ${HEADER_PATH})

add_executable(core_refl_tests tests/reflection_tests.cpp)
target_link_libraries(core_refl_tests PUBLIC malt_core)
