cmake_minimum_required(VERSION 3.5)
project(malt)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/")
set(CMAKE_CXX_STANDARD 14)

include(Malt)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -undefined dynamic_lookup")
endif()

set(HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/malt")
set(HEADER_FILES
        ${HEADER_PATH}/component_mgr.hpp
        ${HEADER_PATH}/component.hpp
        ${HEADER_PATH}/entity.hpp
        ${HEADER_PATH}/module.hpp
        ${HEADER_PATH}/malt_fwd.hpp
        ${HEADER_PATH}/game.hpp
        ${HEADER_PATH}/list.hpp
        ${HEADER_PATH}/engine.hpp
        ${HEADER_PATH}/message.hpp
        ${HEADER_PATH}/utilities.hpp
        ${HEADER_PATH}/game_impl.hpp
        ${HEADER_PATH}/module_impl.hpp
        ${HEADER_PATH}/component_mgr_impl.hpp
        )

set(SOURCE_FILES component.cpp entity.cpp)

add_library(malt_core SHARED ${SOURCE_FILES})
malt_def_module(malt_core)

malt_install(malt_core ${HEADER_PATH})