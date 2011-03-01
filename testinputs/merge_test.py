#!/usr/bin/env python

# contribmerge -- A three-way merge utility for doc/contributions.txt
#
#! @file merge_test.py Test driver for merging
#
# Copyright (C) 2011, Boroondas Gupte
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import sys
import tempfile
import subprocess
import filecmp

contribmerge_command = sys.argv[1]
base_file_name = sys.argv[2]
left_file_name = sys.argv[3]
right_file_name = sys.argv[4]
desired_result_file_name = sys.argv[5]

# Create a temporary file for contribmerge's output. 
# (Will be auto-delted when closed.)
output_file = tempfile.NamedTemporaryFile()

# Call contribmerge.
# (Note the different argument order from above.)
p = subprocess.Popen([ contribmerge_command,
                       '-p',                 # contribmerge output to stdout.
                       left_file_name,       # Won't be overwritten due to -p.
                       base_file_name,
                       right_file_name],
                     stdout=output_file)     # stdout to temporary file.
print p.wait() # Waits for the process to terminate and returns the exit code.

# Compare observed output to expected output
print filecmp.cmp(output_file.name, desired_result_file_name)

# For good measure, close the temporary file (which also deletes it).
# (Both would happen implicitly anyway, as soon as the file object is
# garbage collected.)
output_file.close()
