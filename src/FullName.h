// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file FullName.h Implementation of class FullName.
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

#ifndef FULLNAME_H
#define FULLNAME_H

#include <utility>
#include <string>
#include "Contributions.h"

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

#endif // FULLNAME_H
