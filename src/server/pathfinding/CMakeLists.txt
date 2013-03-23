# Copyright (C) 2010-2011 Izb00shka <http://izbooshka.net/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

if( USE_COREPCH )
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()

file(GLOB sources_localdir *.cpp *.h)

if( USE_COREPCH AND MSVC )
  set(pathfinding_STAT_SRCS
    PrecompiledHeaders/pathfindingPCH.cpp
    PrecompiledHeaders/pathfindingPCH.h
  )
endif()

set(pathfinding_STAT_SRCS
  ${pathfinding_STAT_SRCS}
  ${sources_localdir}
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

add_library(pathfinding STATIC ${pathfinding_STAT_SRCS})

# Generate precompiled header
if( USE_COREPCH )
  if(CMAKE_COMPILER_IS_GNUCXX)
    add_precompiled_header(pathfinding ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders/pathfindingPCH.h)
  elseif(MSVC)
    add_native_precompiled_header(pathfinding ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders/pathfindingPCH)
  endif()
endif()

