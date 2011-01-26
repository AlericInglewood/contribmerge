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
#include "exceptions.h"

FormattedContributions operator+(FormattedContributions const& fc1, FormattedContributions const& fc2)
{
  FormattedContributions result;
  std::set_union(fc1.contributions().begin(),
                 fc1.contributions().end(),
		 fc2.contributions().begin(),
		 fc2.contributions().end(),
		 result.get_inserter(),
		 ContributionEntryCompare());
  return result;
}

Contributor operator+(Contributor const& c1, Contributor const& c2)
{
  assert(c1.first == c2.first);
  return Contributor(c1.first, c1.second + c2.second);
}

template<class Iterator>
inline bool is_less(Iterator const& it1, Iterator const& it2, Iterator const& end1, Iterator const& end2)
{
  return it1 != end1 && (it2 == end2 || ContributionsTxt::contributors_map::key_compare()(*it1, *it2));
}

// Below we use the following notation:
//
// x (y, z) --> r
//
// where x is part of the base, y is part of left and z is part of right: base (left, right) --> result.
//
// If a different symbol is used in the same formula, then that means that that part was changed.
// 
// Left and right are always exchangable. If x (y, z) --> r, then x (z, y) --> r. The latter may be omitted.
// 
// In all cases, x (x, x) --> x,  [if neither side changes anything, nothing is changed]
//               x (x, y) --> y   [if only one side changes something, that is used]
//           and x (y, y) --> y   [if both sides make the same change, who are we to argue]
// These rules are considered trivial and may be omitted in the comments.
//
// When some part is non-existent we use the character '-'.
//
ContributionsTxt merge(ContributionsTxt const& base, ContributionsTxt const& left, ContributionsTxt const& right) throw(MergeFailure)
{
  // Merge the header.
  //
  // The header only follows the trivial rules, plus
  // h1 (h2, h3) --> MergeFailure [if both sides make different changes, fail]
  //
  Header header(base);				// Default in case h (h, h) --> h
  if (header != left && header != right)
  {
    if (left != right)
      throw MergeFailure();			// h1 (h2, h3) --> MergeFailure
    header = left;				// h1 (h2, h2) --> h2
  }
  else if (header != left)
    header = left;				// h1 (h2, h1) --> h2
  else if (header != right)
    header = right;				// h1 (h1, h2) --> h2

  ContributionsTxt result(header);

  // Merge full names.
  //
  // Note that although names are ordered roughly case insensitive,
  // they ARE case sensitive when otherwise equal: "AAA" < "aaa" (lexiographic compare),
  // but "AAb" > "aaa" (case insensitive lexiographic compare). Therefore each name
  // is unique and different names are uncorrelated. Hence, a name either exists (n),
  // or it doesn't exist (-). There are no other possibilities.
  //
  // Therefore, all the rules are trivial.
  // However, we also have the take the payload into account: the list of contributions.
  // Let n, m and l be the same name but different payloads.
  //
  // - (-, -) --> -
  // - (-, n) --> n
  // - (n, -) --> n
  //
  // Case A:
  // - (n, n) --> n
  // - (n, m) --> n + m
  //
  // n (-, -) --> -
  //
  // Case B:
  // n (-, n) --> -
  // n (-, m) --> MergeFailure
  //
  // Case C:
  // n (n, -) --> -
  // n (m, -) --> MergeFailure
  //
  // Case D:
  // n (n, n) --> n
  // n (n, m) --> m
  // n (m, n) --> m
  // n (m, m) --> m
  // n (m, l) --> merge payload
  //

  typedef ContributionsTxt::contributors_map contributors_map;
  typedef contributors_map::const_iterator iterator;
  iterator b  = base.contributors().begin();
  iterator be = base.contributors().end();
  iterator l  = left.contributors().begin();
  iterator le = left.contributors().end();
  iterator r  = right.contributors().begin();
  iterator re = right.contributors().end();

  Inserter<contributors_map> use_it(result.get_inserter());

  // At any point there can be 13 ways that the smallest remaining elements are ordered:
  //
  // Ordering		Action			Rule
  // *b < *l < *r	++b			n (-, -) --> -
  // *b < *r < *l	++b			n (-, -) --> -
  // *l < *b < *r	use_it(*l++)		- (n, -) --> n
  // *l < *r < *b	use_it(*l++)		- (n, -) --> n
  // *r < *b < *l	use_it(*r++)		- (-, n) --> n
  // *r < *l < *b	use_it(*r++)		- (-, n) --> n
  //
  // *r < *b = *l	use_it(*r++)		- (-, n) --> n
  // *b = *l < *r	Case C, ++b, ++l
  // *l < *b = *r	use_it(*l++)		- (n, -) --> n
  // *b = *r < *l	Case B, ++b, ++r
  // *b < *l = *r	++b			n (-, -) --> -
  // *l = *r < *b	Case A, ++r, ++l	- (n, m) --> n + m
  //
  // *b = *l = *r	Case D, ++b, ++l, ++r
  //
  while (b != be || l != le || r != re)
  {
    if (is_less(b, l, be, le))
    {
      // One of
      //
      // Ordering		Action			Rule
      // *b < *l < *r		++b			n (-, -) --> -
      // *b < *r < *l		++b			n (-, -) --> -
      // *r < *b < *l		use_it(*r++)		- (-, n) --> n
      // *b = *r < *l		Case B, ++b, ++r
      // *b < *l = *r		++b			n (-, -) --> -
      //
      if (is_less(b, r, be, re))
      {
	// One of
	//
	// Ordering		Action			Rule
	// *b < *l < *r		++b			n (-, -) --> -
	// *b < *r < *l		++b			n (-, -) --> -
	// *b < *l = *r		++b			n (-, -) --> -
	//
	// Skip it.
	++b;
	continue;
      }
      else if (is_less(r, b, re, be))
      {
	// Ordering		Action			Rule
	// *r < *b < *l		use_it(*r++)		- (-, n) --> n
	//
	// Use it.
	use_it(*r++);
	continue;
      }
      // Ordering		Action			Rule
      // *b = *r < *l		Case B, ++b, ++r
      //
      // Case B

      if (b->second.raw_string() != r->second.raw_string())
      {
	std::cerr << "Failure: hit rule: n (-, m) --> MergeFailure.\n";
	std::cerr << "Where n = <RAW>" << b->second.raw_string() << "</RAW> and m = <RAW>" << r->second.raw_string() << "</RAW>" << std::endl;
	throw MergeFailure();
      }

      ++b;
      ++r;
      continue;
    }
    // One of
    //
    // Ordering		Action			Rule
    // *l < *b < *r	use_it(*l++)		- (n, -) --> n
    // *l < *r < *b	use_it(*l++)		- (n, -) --> n
    // *r < *l < *b	use_it(*r++)		- (-, n) --> n
    // *r < *b = *l	use_it(*r++)		- (-, n) --> n
    // *b = *l < *r	Case C, ++b, ++l
    // *l < *b = *r	use_it(*l++)		- (n, -) --> n
    // *l = *r < *b	Case A, ++r, ++l	- (n, m) --> n + m
    // *b = *l = *r	Case D, ++b, ++l, ++r
    else if (is_less(l, r, le, re))
    {
      // One of
      //
      // Ordering		Action			Rule
      // *l < *b < *r		use_it(*l++)		- (n, -) --> n
      // *l < *r < *b		use_it(*l++)		- (n, -) --> n
      // *b = *l < *r		Case C, ++b, ++l
      // *l < *b = *r		use_it(*l++)		- (n, -) --> n
      if (is_less(l, b, le, be))
      {
	// One of
	//
	// Ordering		Action			Rule
	// *l < *b < *r		use_it(*l++)		- (n, -) --> n
	// *l < *r < *b		use_it(*l++)		- (n, -) --> n
	// *l < *b = *r		use_it(*l++)		- (n, -) --> n
	//
	// Use it.
	use_it(*l++);
	continue;
      }
      // Ordering		Action			Rule
      // *b = *l < *r		Case C, ++b, ++l
      //
      // Case C

      if (b->second.raw_string() != l->second.raw_string())
      {
	std::cerr << "Failure: hit rule: n (m, -) --> MergeFailure.\n";
	std::cerr << "Where n = <RAW>" << b->second.raw_string() << "</RAW> and m = <RAW>" << l->second.raw_string() << "</RAW>" << std::endl;
	throw MergeFailure();
      }

      ++b;
      ++l;
      continue;
    }
    // One of
    //
    // Ordering		Action			Rule
    // *r < *l < *b	use_it(*r++)		- (-, n) --> n
    // *r < *b = *l	use_it(*r++)		- (-, n) --> n
    // *l = *r < *b	Case A, ++r, ++l	- (n, m) --> n + m
    // *b = *l = *r	Case D, ++b, ++l, ++r
    if (is_less(r, l, re, le))
    {
      // One of
      //
      // Ordering		Action			Rule
      // *r < *l < *b		use_it(*r++)		- (-, n) --> n
      // *r < *b = *l		use_it(*r++)		- (-, n) --> n
      //
      // Use it.
      use_it(*r++);
      continue;
    }
    // One of
    //
    // Ordering		Action			Rule
    // *l = *r < *b	Case A, ++r, ++l	- (n, m) --> n + m
    // *b = *l = *r	Case D, ++b, ++l, ++r
    if (is_less(r, b, re, be))
    {
      // Ordering		Action			Rule
      // *l = *r < *b		Case A, ++r, ++l	- (n, m) --> n + m
      //
      // Case A
      //
      // Since n + n = n, we can just always use the union of left and right.
      use_it(*r + *l);

      ++r;
      ++l;
      continue;
    }
    // Ordering		Action			Rule
    // *b = *l = *r	Case D, ++b, ++l, ++r
    //
    // Case D
    // n (n, n) --> n
    // n (n, m) --> m
    // n (m, n) --> m
    // n (m, m) --> m
    // n (m, l) --> merge payload

    if (b->second.raw_string() == r->second.raw_string())
    {
      // One of
      // n (n, n) --> n
      // n (m, n) --> m
      use_it(*l);
    }
    else if (b->second.raw_string() == l->second.raw_string())
    {
      // n (n, m) --> m
      use_it(*r);
    }
    else if (l->second.raw_string() == r->second.raw_string())
    {
      // n (m, m) --> m
      use_it(*r);
    }
    else
    {
      // n (m, l) --> merge payload
      std::cout << "*** payload merge needed ***" << std::endl;
    }

    ++b;
    ++l;
    ++r;
  }

  return result;
}

int main()
{
  Debug(debug::init());

  try
  {
    ContributionsTxt base("base");
    ContributionsTxt left("left");
    ContributionsTxt right("right");

    ContributionsTxt result = merge(base, left, right);
    result.print_on(std::cout);
  }
  catch(ParseError& parse_error)
  {
    std::cerr << "Parsing failed\n" << "Stopped at: \"" << parse_error.rest() << "\"\n";
    exit(1);
  }
  catch(MergeFailure& parse_error)
  {
    std::cerr << "Merge failure\n";
    exit(1);
  }

  return 0;
}

