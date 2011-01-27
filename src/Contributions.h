// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file Contributions.h Implementation of class Contributions.
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

#ifndef CONTRIBUTIONS_H
#define CONTRIBUTIONS_H

#include <vector>
#include <string>
#include "FormattedContributions.h"

// Grammar rule: contributor.
class Contributions
{
  public:
    typedef std::vector<ContributionEntry> contributions_type;

  private:
    std::string M_raw_string;				// Raw contributor data (including full name).
    contributions_type M_contributions;			// Vector of ContributionEntry's.

  public:
    Contributions(void) { }
    Contributions(FormattedContributions const& fc);

    std::insert_iterator<contributions_type> get_inserter(void) { return std::inserter(M_contributions, M_contributions.begin()); }

    // Accessors.
    std::string const& raw_string(void) const { return M_raw_string; }
    contributions_type const& contributions(void) const { return M_contributions; }

  public:
    operator FormattedContributions(void) const { return M_contributions; }
};

#endif // CONTRIBUTIONS_H
