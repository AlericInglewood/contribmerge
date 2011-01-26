// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file ContributionsTxt_operators.h Implementation of operators of ContributionsTxt.
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

#ifndef CONTRIBUTIONSTXT_OPERATORS_H
#define CONTRIBUTIONSTXT_OPERATORS_H

#include "ContributionsTxt.h"

inline bool operator==(ContributionsTxt const& ct1, ContributionsTxt const& ct2)
{
  return ct1.header() == ct2.header();
}

inline bool operator!=(ContributionsTxt const& ct1, ContributionsTxt const& ct2)
{
  return ct1.header() != ct2.header();
}

template<class Container>
struct Inserter {
  std::insert_iterator<Container> M_inserter;
  Inserter(Container& container) : M_inserter(std::inserter(container, container.begin())) { }
  void operator()(typename Container::value_type const& value) { *M_inserter = value; }
};

inline Inserter<ContributionsTxt::contributors_map> ContributionsTxt::get_inserter(void)
{
  return Inserter<contributors_map>(M_contributors);
}

#endif // CONTRIBUTIONSTXT_OPERATORS_H
