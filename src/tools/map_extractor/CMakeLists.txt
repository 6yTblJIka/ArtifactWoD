#
#  ArtifactCore based on TC 6.2.X
#  Copyright (C) 2012-2023 TrinityCore
#  All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
#

file(GLOB_RECURSE sources *.cpp *.h)

include_directories (
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/dep/CascLib/src
  ${CMAKE_SOURCE_DIR}/dep/g3dlite/include
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/loadlib
)

include_directories(${include_Dirs})

add_executable(mapextractor
  ${sources}
)

target_link_libraries(mapextractor
  casc
  g3dlib
  ${BZIP2_LIBRARIES}
  ${ZLIB_LIBRARIES}
)

add_dependencies(mapextractor casc)

if( UNIX )
  install(TARGETS mapextractor DESTINATION bin)
elseif( WIN32 )
  install(TARGETS mapextractor DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()

set_property(TARGET mapextractor PROPERTY FOLDER "tools")