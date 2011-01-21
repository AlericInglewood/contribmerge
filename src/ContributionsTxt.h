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

#include <vector>
#include <string>
#include "exceptions.h"

// Grammar rule: jira_project_key
class JiraProjectKey
{
  private:
    std::string M_jira_project_key_prefix;				// "VWR"
    int M_issue_number;							// 123

  public:
    // Accessors.
    std::string const& jira_project_key_prefix(void) const { return M_jira_project_key_prefix; }
    int issue_number(void) const { return M_issue_number; }
};

// Grammar rule: contribution_entry
class ContributionEntry
{
  private:
    JiraProjectKey M_jira_project_key;					// "VWR-123"
    std::string M_comment;						// Optional (empty if there is none).

  public:
    // Accessors.
    JiraProjectKey const& jira_project_key(void) const { return M_jira_project_key; }
    std::string const& comment(void) const { return M_comment; }
};

// Grammar rule: contributor.
class Contributor
{
  private:
    std::string M_raw_string;						// Raw contributor data.
    std::string M_full_name;						// Firstname[ Lastname].
    std::vector<ContributionEntry> M_contributions;			// Vector of ContributionEntry's.

  public:
    // Accessors.
    std::string const& raw_string(void) const { return M_raw_string; }
    std::string const& full_name(void) const { return M_full_name; }
    std::vector<ContributionEntry> const& contributions(void) const { return M_contributions; }
};

// Grammar rule: contributions_txt.
class ContributionsTxt
{
  private:
    std::string M_header;						// Raw header text.
    std::vector<Contributor> M_contributors;				// Vector of Contributor's.

  public:
    void parse(std::string const& filename) throw(ParseError);
    void print_on(std::ostream& os) const;

    // Accessors.
    std::string const& header(void) const { return M_header; }
    std::vector<Contributor> const& contributors(void) const { return M_contributors; }
};

#endif // CONTRIBUTIONSTXT_H
