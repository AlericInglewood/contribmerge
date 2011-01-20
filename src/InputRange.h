// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file InputRange.h Implementation of class InputRange.
//
// Copyright (C) 2011, Aleric Inglewood & Boroondas Gupte
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

#ifndef INPUTRANGE_H
#define INPUTRANGE_H

#include <utility>

template<class InIt>
struct InputRange
{
  typedef InIt iterator;

  std::pair<InIt, InIt const> M_range;

  InputRange(InIt const& first, InIt const& last = InIt()) : M_range(first, last) { }

  InIt& begin(void) { return M_range.first; }
  InIt const& begin(void) const { return M_range.first; }
  InIt const& end(void) const { return M_range.second; }
  InIt end(void) { return M_range.second; }
  bool empty(void) const { return M_range.first == M_range.second; }
};

#endif // INPUTRANGE_H
