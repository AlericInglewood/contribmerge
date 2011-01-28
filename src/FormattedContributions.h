// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file FormattedContributions.h Implementation of class FormattedContributions.
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

#ifndef FORMATTEDCONTRIBUTIONS_H
#define FORMATTEDCONTRIBUTIONS_H

#include <vector>
#include <set>
#include <iterator>
#include "ContributionEntry.h"

class FormattedContributions
{
  private:
    struct ContributionEntryCompare {
      bool operator()(ContributionEntry const& ce1, ContributionEntry const& ce2) { return ce1.jira_project_key() < ce2.jira_project_key(); }
    };

  public:
    typedef std::set<ContributionEntry, ContributionEntryCompare> contributions_type;

  private:
    contributions_type M_contributions;

  public:
    FormattedContributions(void) { }
    FormattedContributions(std::vector<ContributionEntry> const& vec);

    std::insert_iterator<contributions_type> get_inserter(void) { return std::inserter(M_contributions, M_contributions.begin()); }

    // Accessors.
    contributions_type const& contributions(void) const { return M_contributions; }
};

#endif // FORMATTEDCONTRIBUTIONS_H
