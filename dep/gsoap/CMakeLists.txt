#
#  ArtifactCore based on TC 6.2.X
#  Copyright (C) 2012-2023 TrinityCore
#  All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
#

file(GLOB sources *.cpp *.h)

set(gsoap_STAT_SRCS
   ${sources}
)

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
)

# Little fix for MSVC / Windows platforms
add_definitions(-D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=0)

add_library(gsoap STATIC ${gsoap_STAT_SRCS})

set_property(TARGET gsoap PROPERTY FOLDER "dep")