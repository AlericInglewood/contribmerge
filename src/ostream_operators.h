// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file ostream_operators.h Declaration of various ostream inserter functions.
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

#ifndef OSTREAM_OPERATORS_H
#define OSTREAM_OPERATORS_H

#ifndef USE_PCH
#include <iosfwd>		// Needed for std::ostream
#endif

class JiraProjectKey;
class ContributionEntry;
class Contributions;
class FullName;
class ContributionsTxt;
typedef std::pair<FullName const, Contributions> Contributor;

std::ostream& operator<<(std::ostream& os, JiraProjectKey const& key);
std::ostream& operator<<(std::ostream& os, ContributionEntry const& entry);
std::ostream& operator<<(std::ostream& os, FullName const& full_name);
std::ostream& operator<<(std::ostream& os, Contributions const& contributions);
std::ostream& operator<<(std::ostream& os, ContributionsTxt const& contributions_txt);
std::ostream& operator<<(std::ostream& os, Contributor const&);

#endif // OSTREAM_OPERATORS_H
