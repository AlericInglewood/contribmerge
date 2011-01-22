// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file ostream_operators.cc Implementation of various ostream inserter functions.
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

#ifndef USE_PCH
#include "sys.h"
#include <iostream>
#include "debug.h"
#endif

#include "ostream_operators.h"
#include "ContributionsTxt.h"

//-----------------------------------------------------------------------------
//
// ostream operator<<'s
//

std::ostream& operator<<(std::ostream& os, JiraProjectKey const& key)
{
  os << key.jira_project_key_prefix();
  if (key.issue_number())
    os << '-' << key.issue_number();
  return os;
}

std::ostream& operator<<(std::ostream& os, ContributionEntry const& entry)
{
  os << entry.jira_project_key();
  if (!entry.comment().empty())
    os << ' ' << entry.comment();
  return os;
}

std::ostream& operator<<(std::ostream& os, Contributions const& contributions)
{
  //os << "<RAW>" << contributions.raw_string() << "</RAW>\n";
  for (std::vector<ContributionEntry>::const_iterator contribution = contributions.contributions().begin(); contribution != contributions.contributions().end(); ++contribution)
  {
    os << '\t' << *contribution << "\n";
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, FullName const& full_name)
{
  os << full_name.full_name();
  return os;
}

std::ostream& operator<<(std::ostream& os, ContributionsTxt const& contributions_txt)
{
  os << "Header:=======================================\n" << contributions_txt.header() << "==============================================\n\n";
  for (std::map<FullName, Contributions>::const_iterator contributor = contributions_txt.contributors().begin(); contributor != contributions_txt.contributors().end(); ++contributor)
  {
    os << contributor->first << '\n';
    os << contributor->second;
  }
  os << "Number of Contributors: " << contributions_txt.contributors().size() << '\n';
  return os;
}

