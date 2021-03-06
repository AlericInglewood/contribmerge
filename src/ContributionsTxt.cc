// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file ContributionsTxt.cc Implementation of class ContributionsTxt.
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

#ifndef USE_PCH
#include "sys.h"
#include <fstream>
#include <iomanip>
#include "debug.h"
#endif

#include "ContributionsTxt.h"
#include "grammar_contrib.h"
#include "ostream_operators.h"

ContributionsTxt::ContributionsTxt(std::string const& filename) throw(ParseError) : M_header(std::string())
{
  std::ifstream infile(filename.c_str());
  typedef boost::spirit::istream_iterator parse_iterator_type;
  InputRange<parse_iterator_type> range(static_cast<parse_iterator_type>(infile));
  infile.unsetf(std::ios::skipws);

  bool success = boost::spirit::qi::parse(
      range.begin(), range.end(),
      grammar::contributions_txt_grammar<parse_iterator_type>(),
      *this);

  if (!(success && range.empty()))
    throw ParseError(range);
}

void ContributionsTxt::print_on(std::ostream& os) const
{
  os << *this;
}
