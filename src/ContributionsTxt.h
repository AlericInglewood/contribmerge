// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file ContributionsTxt.h Implementation of class ContributionsTxt.
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

#ifndef CONTRIBUTIONSTXT_H
#define CONTRIBUTIONSTXT_H

#include "contrib_grammar.h"
#include "InputRange.h"
#include <exception>

class ParseError : public std::exception
{
  public:
    // Constructor.
    template<class InIt>
      ParseError(InputRange<InIt> const& bounded_input_range);
    // Destructor.
    virtual ~ParseError() throw() { }

    std::string const& rest(void) const { return M_rest; }

  private:
    std::string M_rest;
};

class ContributionsTxt : private attributes::ContributionsTxt
{
  public:
    void parse(std::string const& filename) throw(ParseError);
    void print_on(std::ostream& os) const;
};

#endif // CONTRIBUTIONSTXT_H
