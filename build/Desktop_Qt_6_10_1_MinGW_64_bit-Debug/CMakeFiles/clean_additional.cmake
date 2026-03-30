# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Itog_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Itog_autogen.dir\\ParseCache.txt"
  "Itog_autogen"
  )
endif()
