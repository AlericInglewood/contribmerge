// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file grammar_attributes.h Declaration of namespace attributes.
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

#ifndef GRAMMAR_ATTRIBUTES_H
#define GRAMMAR_ATTRIBUTES_H

#include <string>
#include <vector>

namespace attributes {

// Rule: jira_project_key
struct JiraProjectKey
{
  std::string jira_project_key_prefix;		// "VWR"
  int issue_number;				// 123
};

// Rule: contribution_entry
struct ContributionEntry
{
  JiraProjectKey jira_project_key;		// "VWR-123"
  std::string comment;				// Optional (empty if there is none).
};

// Rule: contributor
struct Contributor
{
  std::string raw_string;
  std::string full_name;			// "Firstname[ Lastname]"
  std::vector<ContributionEntry> contributions;
};

// The type I want to parse the data into:
struct ContributionsTxt
{
  std::string header;				// Raw header text.
  std::vector<Contributor> contributors;
};

} // namespace attributes

#endif // GRAMMAR_ATTRIBUTES_H
