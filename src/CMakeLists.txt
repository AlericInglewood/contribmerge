# contribmerge -- A three-way merge utility for doc/contributions.txt
#
# Copyright (C) 2011, Aleric Inglewood & Boroondas Gupte
#
# This build script is free software. It comes without any warranty, to
# the extent permitted by applicable law. You can redistribute it
# and/or modify it under the terms of the Do What The Fuck You Want
# To Public License, Version 2, as published by Sam Hocevar. See
# LICENSE.WTFPL2 or http://sam.zoy.org/wtfpl/COPYING for more details.

# mangle header file
configure_file (
  "sys.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/sys.h"
  )

# make it possible to find the generated sys.h
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

#* \TODO: how can we tell find_package that we don't need multi-thread
#*        support?
find_package(Boost COMPONENTS program_options REQUIRED)

include_directories(SYSTEM ${Boost_INCLUDE_DIR})

add_executable(contribmerge contribmerge.cc Contributions.cc ContributionsTxt.cc debug.cc FormattedContributions.cc FullName.cc Header.cc ostream_operators.cc)
target_link_libraries(contribmerge ${Boost_LIBRARIES})
