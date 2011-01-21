// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file ContributionsTxt.cc Implementation of class ContributionsTxt.
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
#include <string>
#include <fstream>
#include <iomanip>
#include "debug.h"
#endif

#include "ContributionsTxt.h"
#include "ostream_operators.h"

template<class InIt>
ParseError::ParseError(InputRange<InIt> const& bounded_input_range)
{
  int count = 100;
  for (typename InputRange<InIt>::iterator iter = bounded_input_range.begin(); iter != bounded_input_range.end(); ++iter)
  {
    if (--count == 0)
    {
      M_rest += "...";
      break;
    }
    M_rest += *iter;
  }
}

void ContributionsTxt::parse(std::string const& filename) throw(ParseError)
{
  std::ifstream infile(filename.c_str());
  typedef boost::spirit::istream_iterator parse_iterator_type;
  InputRange<parse_iterator_type> range(static_cast<parse_iterator_type>(infile));
  infile.unsetf(std::ios::skipws);

  bool success = boost::spirit::qi::parse(
      range.begin(), range.end(),
      grammar::contributions_txt_grammar<parse_iterator_type>(),
      static_cast<attributes::ContributionsTxt&>(*this));

  if (!(success && range.empty()))
    throw ParseError(range);
}

void ContributionsTxt::print_on(std::ostream& os) const
{
  attributes::ContributionsTxt const& attributes(*this);
  os << attributes;
}