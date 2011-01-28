// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file JiraProjectKey.h Implementation of class JiraProjectKey.
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

#ifndef JIRAPROJECTKEY_H
#define JIRAPROJECTKEY_H

#include <string>

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

#endif // JIRAPROJECTKEY_H
