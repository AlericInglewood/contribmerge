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

#include <map>
#include <vector>
#include <string>
#include <boost/algorithm/string/predicate.hpp>
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
class Contributions
{
  private:
    std::string M_raw_string;						// Raw contributor data (including full name).
    std::vector<ContributionEntry> M_contributions;			// Vector of ContributionEntry's.

  public:
    // Accessors.
    std::string const& raw_string(void) const { return M_raw_string; }
    std::vector<ContributionEntry> const& contributions(void) const { return M_contributions; }
};

class Header
{
  private:
    std::string M_header;						// Raw header data.

  public:
    Header(void) { }
    Header(std::string const& header) : M_header(header) { }

    // Accessors.
    std::string const& as_string(void) const { return M_header; }

    // Operator.
    friend bool operator==(Header const& h1, Header const& h2) { return h1.M_header == h2.M_header; }
    friend bool operator!=(Header const& h1, Header const& h2) { return h1.M_header != h2.M_header; }
};

class FullName
{
  private:
    std::string M_full_name;						// Firstname[ Lastname].

  public:
    FullName(std::string const& full_name) : M_full_name(full_name) { }

    // Accessors.
    std::string const& full_name(void) const { return M_full_name; }

  public:
    typedef std::pair<FullName const, Contributions> contributors_map_value_type;
    struct CaseInsensitiveCompare {
      bool operator()(FullName const& name1, FullName const& name2) const;
      bool operator()(contributors_map_value_type const& name1, contributors_map_value_type const& name2) const
          { return operator()(name1.first, name2.first); }
    };
};

// Forward declaration.
template<int ctop>
struct ContributionsTxtOperator;

// Grammar rule: contributions_txt.
class ContributionsTxt
{
  public:
    typedef std::map<FullName, Contributions, FullName::CaseInsensitiveCompare> contributors_map;

  private:
    Header M_header;							// Raw header text.
    contributors_map M_contributors;					// Map of Contributors.

  public:
    void parse(std::string const& filename) throw(ParseError);
    void print_on(std::ostream& os) const;

    // Accessors.
    Header const& header(void) const { return M_header; }
    contributors_map const& contributors(void) const { return M_contributors; }

    // Assignment operators.
    template<int ctop>
    ContributionsTxt& operator=(ContributionsTxtOperator<ctop> const& args);
};

#endif // CONTRIBUTIONSTXT_H
