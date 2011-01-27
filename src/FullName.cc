// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file FullName.cc Implementation(s) related to class FullName.
//
// Copyright (C) 2011, Aleric Inglewood
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef USE_PCH
#include "sys.h"
#include <algorithm>
#include "debug.h"
#endif

#include "FullName.h"
#include <boost/algorithm/string/predicate.hpp>

bool FullName::Compare::operator()(FullName const& name1, FullName const& name2) const
{
  std::string const& n1(name1.M_full_name);
  std::string const& n2(name2.M_full_name);
  if (n1 == n2)
    return false;
  if (boost::ilexicographical_compare(n1, n2))
    return true;
  if (boost::ilexicographical_compare(n2, n1))
    return false;
  return boost::lexicographical_compare(n1, n2);
}
