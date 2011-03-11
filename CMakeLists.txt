# contribmerge -- A three-way merge utility for doc/contributions.txt
#
# Copyright (C) 2011, Aleric Inglewood & Boroondas Gupte
#
# This build script is free software. It comes without any warranty, to
# the extent permitted by applicable law. You can redistribute it
# and/or modify it under the terms of the Do What The Fuck You Want
# To Public License, Version 2, as published by Sam Hocevar. See
# LICENSE.WTFPL2 or http://sam.zoy.org/wtfpl/COPYING for more details.

cmake_minimum_required (VERSION 2.6)
project (ContribMerge)

enable_testing()

add_subdirectory(src)
add_subdirectory(testinputs)
