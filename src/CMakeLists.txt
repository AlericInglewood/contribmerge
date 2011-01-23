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

add_executable(contribmerge contribmerge.cc ostream_operators.cc ContributionsTxt.cc ContributionsTxt_operators.cc)
