cmake_minimum_required(VERSION 3.2)
project(malt)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../malt_cmake/")
set(CMAKE_CXX_STANDARD 14)

include(Malt)

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
        ${HEADER_PATH}/engine_impl.hpp
        ${HEADER_PATH}/track_ptr.hpp
        ${HEADER_PATH}/strided_iterator.hpp
        ${HEADER_PATH}/malt_reflect.hpp
        ${HEADER_PATH}/malt_reflect_impl.hpp
        )

set(SOURCE_FILES src/component.cpp src/entity.cpp include/malt/detail/entity_info.hpp src/utilities.cpp)

add_library(malt_core ${MALT_LIB_TYPE} ${SOURCE_FILES})
malt_def_module(malt_core)

set_target_properties(malt_core PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(malt_core PROPERTIES C_VISIBILITY_PRESET hidden)

malt_install(malt_core ${HEADER_PATH})

add_executable(core_refl_tests tests/reflection_tests.cpp)
target_link_libraries(core_refl_tests PUBLIC malt_core)