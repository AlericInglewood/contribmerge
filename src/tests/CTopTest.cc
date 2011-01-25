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

bool names_is_equal(ContributionsTxt const& c1, ContributionsTxt const& c2)
{
  ContributionsTxt::contributors_map::const_iterator i1 = c1.contributors().begin();
  ContributionsTxt::contributors_map::const_iterator e1 = c1.contributors().end();
  ContributionsTxt::contributors_map::const_iterator i2 = c2.contributors().begin();
  ContributionsTxt::contributors_map::const_iterator e2 = c2.contributors().end();
  for(;;)
  {
    if (i1 == e1 || i2 == e2)
      return i1 == e1 && i2 == e2;
    if (i1->first != i2->first)
      return false;
    ++i1;
    ++i2;
  }
}

#define STRINGIFY(x) STRINGIFY1(x)
#define STRINGIFY1(x) #x

int main()
{
  Debug(debug::init());

  try
  {
    std::string testinputsdir = STRINGIFY(SRCDIR) "/../../testinputs";
    ContributionsTxt base(testinputsdir + "/CTop_base.txt");
    ContributionsTxt left(testinputsdir + "/CTop_left.txt");
    ContributionsTxt right(testinputsdir + "/CTop_right.txt");
    std::cout << "Parsing succeeded\n";

    ContributionsTxt result1(base.header()), result2(base.header());

    result1 = base + left;
    result2 = base;
    result2 += left;
    assert(names_is_equal(result1, result2));
    result2 = left;
    result2 = base + result2;
    assert(names_is_equal(result1, result2));

    result1 = left + base;
    result2 = left;
    result2 += base;
    assert(names_is_equal(result1, result2));
    result2 = base;
    result2 = left + result2;
    assert(names_is_equal(result1, result2));

    result1 = base - left;
    result2 = base;
    result2 -= left;
    assert(names_is_equal(result1, result2));
    result2 = left;
    result2 = base - result2;
    assert(names_is_equal(result1, result2));

    result1 = left - base;
    result2 = left;
    result2 -= base;
    assert(names_is_equal(result1, result2));
    result2 = base;
    result2 = left - result2;
    assert(names_is_equal(result1, result2));

    result1 = base ^ left;
    result2 = base;
    result2 ^= left;
    assert(names_is_equal(result1, result2));
    result2 = left;
    result2 = base ^ result2;
    assert(names_is_equal(result1, result2));

    result1 = left ^ base;
    result2 = left;
    result2 ^= base;
    assert(names_is_equal(result1, result2));
    result2 = base;
    result2 = left ^ result2;
    assert(names_is_equal(result1, result2));

    result1 = base & left;
    result2 = base;
    result2 &= left;
    assert(names_is_equal(result1, result2));
    result2 = left;
    result2 = base & result2;
    assert(names_is_equal(result1, result2));

    result1 = left & base;
    result2 = left;
    result2 &= base;
    assert(names_is_equal(result1, result2));
    result2 = base;
    result2 = left & result2;
    assert(names_is_equal(result1, result2));

    result1 = base + right;
    result2 = base;
    result2 += right;
    assert(names_is_equal(result1, result2));
    result2 = right;
    result2 = base + result2;
    assert(names_is_equal(result1, result2));

    result1 = right + base;
    result2 = right;
    result2 += base;
    assert(names_is_equal(result1, result2));
    result2 = base;
    result2 = right + result2;
    assert(names_is_equal(result1, result2));

    result1 = base - right;
    result2 = base;
    result2 -= right;
    assert(names_is_equal(result1, result2));
    result2 = right;
    result2 = base - result2;
    assert(names_is_equal(result1, result2));

    result1 = right - base;
    result2 = right;
    result2 -= base;
    assert(names_is_equal(result1, result2));
    result2 = base;
    result2 = right - result2;
    assert(names_is_equal(result1, result2));

    result1 = base ^ right;
    result2 = base;
    result2 ^= right;
    assert(names_is_equal(result1, result2));
    result2 = right;
    result2 = base ^ result2;
    assert(names_is_equal(result1, result2));

    result1 = right ^ base;
    result2 = right;
    result2 ^= base;
    assert(names_is_equal(result1, result2));
    result2 = base;
    result2 = right ^ result2;
    assert(names_is_equal(result1, result2));

    result1 = base & right;
    result2 = base;
    result2 &= right;
    assert(names_is_equal(result1, result2));
    result2 = right;
    result2 = base & result2;
    assert(names_is_equal(result1, result2));

    result1 = right & base;
    result2 = right;
    result2 &= base;
    assert(names_is_equal(result1, result2));
    result2 = base;
    result2 = right & result2;
    assert(names_is_equal(result1, result2));

    result1 = left + right;
    result2 = left;
    result2 += right;
    assert(names_is_equal(result1, result2));
    result2 = right;
    result2 = left + result2;
    assert(names_is_equal(result1, result2));

    result1 = right + left;
    result2 = right;
    result2 += left;
    assert(names_is_equal(result1, result2));
    result2 = left;
    result2 = right + result2;
    assert(names_is_equal(result1, result2));

    result1 = left - right;
    result2 = left;
    result2 -= right;
    assert(names_is_equal(result1, result2));
    result2 = right;
    result2 = left - result2;
    assert(names_is_equal(result1, result2));

    result1 = right - left;
    result2 = right;
    result2 -= left;
    assert(names_is_equal(result1, result2));
    result2 = left;
    result2 = right - result2;
    assert(names_is_equal(result1, result2));

    result1 = left ^ right;
    result2 = left;
    result2 ^= right;
    assert(names_is_equal(result1, result2));
    result2 = right;
    result2 = left ^ result2;
    assert(names_is_equal(result1, result2));

    result1 = right ^ left;
    result2 = right;
    result2 ^= left;
    assert(names_is_equal(result1, result2));
    result2 = left;
    result2 = right ^ result2;
    assert(names_is_equal(result1, result2));

    result1 = left & right;
    result2 = left;
    result2 &= right;
    assert(names_is_equal(result1, result2));
    result2 = right;
    result2 = left & result2;
    assert(names_is_equal(result1, result2));

    result1 = right & left;
    result2 = right;
    result2 &= left;
    assert(names_is_equal(result1, result2));
    result2 = left;
    result2 = right & result2;
    assert(names_is_equal(result1, result2));

    result2 = left;
    result1 = result2 + left;
    result2 += result2;
    assert(names_is_equal(result1, result2));

    result2 = right;
    result1 = result2 + right;
    result2 += result2;
    assert(names_is_equal(result1, result2));

    result2 = base;
    result1 = result2 + base;
    result2 += result2;
    assert(names_is_equal(result1, result2));

    result2 = left;
    result1 = result2 - left;
    result2 -= result2;
    assert(names_is_equal(result1, result2));

    result2 = right;
    result1 = result2 - right;
    result2 -= result2;
    assert(names_is_equal(result1, result2));

    result2 = base;
    result1 = result2 - base;
    result2 -= result2;
    assert(names_is_equal(result1, result2));

    result2 = left;
    result1 = result2 ^ left;
    result2 ^= result2;
    assert(names_is_equal(result1, result2));

    result2 = right;
    result1 = result2 ^ right;
    result2 ^= result2;
    assert(names_is_equal(result1, result2));

    result2 = base;
    result1 = result2 ^ base;
    result2 ^= result2;
    assert(names_is_equal(result1, result2));

    result2 = left;
    result1 = result2 & left;
    result2 &= result2;
    assert(names_is_equal(result1, result2));

    result2 = right;
    result1 = result2 & right;
    result2 &= result2;
    assert(names_is_equal(result1, result2));

    result2 = base;
    result1 = result2 & base;
    result2 &= result2;
    assert(names_is_equal(result1, result2));

    std::cout << "Success!\n";
  }
  catch(ParseError& parse_error)
  {
    std::cout << "Parsing failed\n" << "Stopped at: \"" << parse_error.rest() << "\"\n";
    exit(1);
  }

  return 0;
}
