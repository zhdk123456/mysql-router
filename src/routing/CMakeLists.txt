# Copyright (c) 2015, 2016, Oracle and/or its affiliates. All rights reserved.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

set(ROUTING_SOURCE_FILES_X_PROTOCOL
  ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol/x_protocol.cc
)

set(ROUTING_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/mysql_routing.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/destination.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/dest_metadata_cache.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/dest_first_available.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/routing.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol/classic_protocol.cc
  ${ROUTING_SOURCE_FILES_X_PROTOCOL}
)

set(ROUTING_PLUGIN_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/routing_plugin.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/plugin_config.cc
)

set(include_dirs
  ${CMAKE_SOURCE_DIR}/mysql_harness/plugins/logger/include
  ${CMAKE_SOURCE_DIR}/src/router/include
  ${CMAKE_SOURCE_DIR}/src/routing/include
  ${CMAKE_SOURCE_DIR}/src/metadata_cache/include
  ${CMAKE_SOURCE_DIR}/src/mysql_protocol/include
  ${CMAKE_SOURCE_DIR}/src/x_protocol/include
  ${PROTOBUF_INCLUDE_DIR}
  ${CMAKE_BINARY_DIR}/generated/protobuf
)

# this file includes protobuf generated header that is causing 'shadow' warning on some compilers
check_cxx_compiler_flag("-Wshadow" CXX_HAVE_SHADOW)
if(CXX_HAVE_SHADOW)
  add_compile_flags(${CMAKE_CURRENT_SOURCE_DIR}/src/protocol/x_protocol.cc
    COMPILE_FLAGS "-Wno-shadow")
endif()
check_cxx_compiler_flag("-Wsign-conversion" CXX_HAVE_SIGN_CONVERSION)
if(CXX_HAVE_SIGN_CONVERSION)
  add_compile_flags(${CMAKE_CURRENT_SOURCE_DIR}/src/protocol/x_protocol.cc
    COMPILE_FLAGS "-Wno-sign-conversion")
endif()
check_cxx_compiler_flag("-Wunused-parameter" CXX_HAVE_UNUSED_PARAMETER)
if(CXX_HAVE_UNUSED_PARAMETER)
  add_compile_flags(${CMAKE_CURRENT_SOURCE_DIR}/src/protocol/x_protocol.cc
    COMPILE_FLAGS "-Wno-unused-parameter")
endif()
check_cxx_compiler_flag("-Wdeprecated-declarations" CXX_HAVE_DEPRECATED_DECLARATIONS)
if(CXX_HAVE_DEPRECATED_DECLARATIONS)
  add_compile_flags(${CMAKE_CURRENT_SOURCE_DIR}/src/protocol/x_protocol.cc
    COMPILE_FLAGS "-Wno-deprecated-declarations")
endif()

link_directories(${CMAKE_BINARY_DIR}/ext/protobuf/protobuf-3.0.0/cmake/)
# The Plugin
add_harness_plugin(routing
        SOURCES ${ROUTING_PLUGIN_SOURCE_FILES} ${ROUTING_SOURCE_FILES}
        REQUIRES logger mysql_protocol x_protocol metadata_cache)
target_include_directories(routing PRIVATE ${include_dirs})

if(MSVC)
  add_compile_flags(${PROTO_SRCS} COMPILE_FLAGS "/wd4018")
  add_compile_flags(${ROUTING_SOURCE_FILES_X_PROTOCOL} COMPILE_FLAGS "/DX_PROTOCOL_DEFINE_DYNAMIC"
                    "/FImysqlrouter/xprotocol.h")
else()
  add_compile_flags(${ROUTING_SOURCE_FILES_X_PROTOCOL} COMPILE_FLAGS
                    "-include mysqlrouter/xprotocol.h")
endif(MSVC)

target_link_libraries(routing PRIVATE ${PB_LIBRARY})

if(${CMAKE_SYSTEM_NAME} STREQUAL "SunOS")
  target_link_libraries(routing PRIVATE -lnsl PRIVATE -lsocket)
endif()

file(GLOB routing_headers include/mysqlrouter/*.h)
install(FILES ${routing_headers}
  DESTINATION "include/mysql/${HARNESS_NAME}")

if(ENABLE_TESTS)
  add_subdirectory(tests/)
endif()
