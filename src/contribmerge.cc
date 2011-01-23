// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file contribmerge.cc Main implementation.
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

#include "contribmerge.h"
#include "ContributionsTxt.h"
#include "ContributionsTxt_operators.h"

int main()
{
  Debug(debug::init());

  ContributionsTxt contributions_txt1;
  ContributionsTxt contributions_txt2;
  try
  {
    contributions_txt1.parse("testinput1");
    contributions_txt2.parse("testinput2");
    std::cout << "Parsing succeeded\n";
  }
  catch(ParseError& parse_error)
  {
    std::cout << "Parsing failed\n" << "Stopped at: \"" << parse_error.rest() << "\"\n";
    exit(1);
  }

  ContributionsTxt contributions_txt3;

  contributions_txt3 = contributions_txt1;
  contributions_txt3 = contributions_txt3 ^ contributions_txt2;
  std::cout << contributions_txt3 << std::endl;

  return 0;
}

