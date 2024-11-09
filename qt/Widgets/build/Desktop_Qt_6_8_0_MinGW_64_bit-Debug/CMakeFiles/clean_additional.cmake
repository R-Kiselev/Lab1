# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Widgets_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Widgets_autogen.dir\\ParseCache.txt"
  "Widgets_autogen"
  )
endif()
