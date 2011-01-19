// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file ostream_operators.h Declaration of various ostream inserter functions.
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

#ifndef OSTREAM_OPERATORS_H
#define OSTREAM_OPERATORS_H

#ifndef USE_PCH
#include <iosfwd>		// Needed for std::ostream
#endif

#include "ContribGrammar.h"

std::ostream& operator<<(std::ostream& os, attributes::JiraProjectKey const& key)
{
  os << key.jira_project_key_prefix;
  if (key.issue_number)
    os << '-' << key.issue_number;
  return os;
}

std::ostream& operator<<(std::ostream& os, attributes::ContributionEntry const& entry)
{
  os << entry.jira_project_key;
  if (!entry.comment.empty())
    os << ' ' << entry.comment;
  return os;
}

std::ostream& operator<<(std::ostream& os, attributes::Contributor const& contributor)
{
  os << contributor.full_name << "\\n\n";
  for (std::vector<attributes::ContributionEntry>::const_iterator iter = contributor.contributions.begin(); iter != contributor.contributions.end(); ++iter)
  {
    os << '\t' << *iter << "\\n\n";
  }
  return os;
}

#endif // OSTREAM_OPERATORS_H
