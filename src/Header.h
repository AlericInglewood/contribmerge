// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file Header.h Implementation of class Header.
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

#ifndef HEADER_H
#define HEADER_H

#include <string>

class ContributionsTxt;

class Header
{
  private:
    std::string M_header;	// Raw header data.

  public:
    explicit Header(std::string const& header) : M_header(header) { }
    explicit Header(ContributionsTxt const&);
    Header& operator=(ContributionsTxt const&);

    // Accessors.
    std::string const& as_string(void) const { return M_header; }

    // Operator.
    friend bool operator==(Header const& h1, Header const& h2) { return h1.M_header == h2.M_header; }
    friend bool operator!=(Header const& h1, Header const& h2) { return h1.M_header != h2.M_header; }
};

#endif // HEADER_H
