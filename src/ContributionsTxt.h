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
#include <set>
#include <iterator>
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

    friend bool operator<(JiraProjectKey const& jpk1, JiraProjectKey const& jpk2)
    {
      if (jpk1.M_jira_project_key_prefix < jpk2.M_jira_project_key_prefix)
	return true;
      else if (jpk2.M_jira_project_key_prefix < jpk1.M_jira_project_key_prefix)
	return false;
      return jpk1.M_issue_number < jpk2.M_issue_number;
    }
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

struct ContributionEntryCompare {
  bool operator()(ContributionEntry const& ce1, ContributionEntry const& ce2)
  {
    return ce1.jira_project_key() < ce2.jira_project_key();
  }
};

class FormattedContributions
{
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

class ContributionsTxt;

class Header
{
  private:
    std::string M_header;						// Raw header data.

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

class FullName;

typedef std::pair<FullName const, Contributions> Contributor;

class FullName
{
  private:
    std::string M_full_name;						// Firstname[ Lastname].

  public:
    FullName(std::string const& full_name) : M_full_name(full_name) { }

    // Accessors.
    std::string const& full_name(void) const { return M_full_name; }

  public:
    friend bool operator==(FullName const& name1, FullName const& name2) { return name1.M_full_name == name2.M_full_name; }
    friend bool operator!=(FullName const& name1, FullName const& name2) { return name1.M_full_name != name2.M_full_name; }

    struct Compare {
      bool operator()(FullName const& name1, FullName const& name2) const;
      bool operator()(Contributor const& name1, Contributor const& name2) const { return operator()(name1.first, name2.first); }
    };
};

template<class Container> struct Inserter;

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

    Inserter<contributors_map> get_inserter(void);

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
};

#endif // CONTRIBUTIONSTXT_H
