# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\QtSpriteEdit_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\QtSpriteEdit_autogen.dir\\ParseCache.txt"
  "QtSpriteEdit_autogen"
  )
endif()
