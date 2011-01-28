// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file ContributionsTxt.h Implementation of class ContributionsTxt.
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

#ifndef CONTRIBUTIONSTXT_H
#define CONTRIBUTIONSTXT_H

#include <map>
#include <string>
#include "exceptions.h"
#include "FullName.h"
#include "Header.h"

// Grammar rule: contributions_txt.
class ContributionsTxt
{
  public:
    typedef std::map<FullName, Contributions, FullName::Compare> contributors_map;

  private:
    Header M_header;							// Raw header text.
    contributors_map M_contributors;					// Map of Contributors.

  public:
    ContributionsTxt(std::string const& filename) throw(ParseError);
    explicit ContributionsTxt(Header const& header) : M_header(header) { }
    void print_on(std::ostream& os) const;

    std::insert_iterator<contributors_map> get_inserter(void) { return std::inserter<contributors_map>(M_contributors, M_contributors.begin()); }

    // Accessors.
    Header const& header(void) const { return M_header; }
    contributors_map const& contributors(void) const { return M_contributors; }

    // Operators.
    ContributionsTxt& operator=(Header const& header) throw() { M_header = header; return *this; }
    ContributionsTxt& operator=(ContributionsTxt const& ct) throw() { M_contributors = ct.M_contributors; return *this; }

    friend bool operator==(ContributionsTxt const& ct, Header const& header) { return ct.M_header == header; }
    friend bool operator==(Header const& header, ContributionsTxt const& ct) { return header == ct.M_header; }
    friend bool operator!=(ContributionsTxt const& ct, Header const& header) { return ct.M_header != header; }
    friend bool operator!=(Header const& header, ContributionsTxt const& ct) { return header != ct.M_header; }
    friend bool operator==(ContributionsTxt const& ct1, ContributionsTxt const& ct2) { return ct1.M_header == ct2.M_header; }
    friend bool operator!=(ContributionsTxt const& ct1, ContributionsTxt const& ct2) { return ct1.M_header != ct2.M_header; }
};

#endif // CONTRIBUTIONSTXT_H
