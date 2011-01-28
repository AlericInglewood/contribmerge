// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file ContributionEntry.h Implementation of class ContributionEntry.
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

#ifndef CONTRIBUTIONENTRY_H
#define CONTRIBUTIONENTRY_H

#include <string>
#include "JiraProjectKey.h"

// Grammar rule: contribution_entry
class ContributionEntry
{
  private:
    JiraProjectKey M_jira_project_key;		// "VWR-123"
    std::string M_comment;			// Optional (empty if there is none).

  public:
    // Accessors.
    JiraProjectKey const& jira_project_key(void) const { return M_jira_project_key; }
    std::string const& comment(void) const { return M_comment; }
};

#endif // CONTRIBUTIONENTRY_H
