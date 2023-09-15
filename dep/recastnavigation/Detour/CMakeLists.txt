#
#  ArtifactCore based on TC 6.2.X
#  Copyright (C) 2012-2023 TrinityCore
#  All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
#

set(Detour_STAT_SRCS
    Source/DetourAlloc.cpp
    Source/DetourCommon.cpp
    Source/DetourNavMesh.cpp
    Source/DetourNavMeshBuilder.cpp
    Source/DetourNavMeshQuery.cpp
    Source/DetourNode.cpp
)
include_directories(Include)

if(WIN32)
  include_directories(
    ${CMAKE_SOURCE_DIR}/dep/zlib
  )
endif()

add_library(Detour STATIC ${Detour_STAT_SRCS})

target_link_libraries(Detour ${ZLIB_LIBRARIES})

set_property(TARGET Detour PROPERTY FOLDER "dep")
