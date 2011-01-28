// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file Contributions.cc Implementation of class Contributions.
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

#include "Contributions.h"
#include "Inserter.h"

Contributions::Contributions(FormattedContributions const& fc)
{
  Inserter<contributions_type> insert_into_contributions(M_contributions);
  for_each(fc.contributions().begin(), fc.contributions().end(), insert_into_contributions);
}
