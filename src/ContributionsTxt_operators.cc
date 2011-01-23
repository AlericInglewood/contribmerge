// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file ContributionsTxt_operators.cc Implementation of operators of ContributionsTxt.
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
#include <algorithm>
#include "debug.h"
#endif

#include "ContributionsTxt_operators.h"
#include "ostream_operators.h"

bool Contributions::raw_compare(Contributions const& contribution1, Contributions const& contribution2)
{
  bool result = contribution1.raw_string() < contribution2.raw_string();
  return result;
}

bool ContributionsTxt::full_compare::operator()(contributors_map::value_type const& contributor1, contributors_map::value_type const& contributor2) const
{
  static FullName::CaseInsensitiveCompare name_compare;
  if (name_compare(contributor1.first, contributor2.first))
    return true;
  if (name_compare(contributor2.first, contributor1.first))
    return false;
  return Contributions::raw_compare(contributor1.second, contributor2.second);
}

bool FullName::CaseInsensitiveCompare::operator()(FullName const& name1, FullName const& name2) const
{
  std::string const& n1(name1.M_full_name);
  std::string const& n2(name2.M_full_name);
  if (n1 == n2)
    return false;
  if (boost::ilexicographical_compare(n1, n2))
    return true;
  if (boost::ilexicographical_compare(n2, n1))
    return false;
  return boost::lexicographical_compare(n1, n2);
}

template<>
ContributionsTxt& ContributionsTxt::operator=(ContributionsTxtOperator<ctop_intersection> const& args)
{
  ContributionsTxt const* arg1 = &args.M_ct1;
  ContributionsTxt const* arg2 = &args.M_ct2;
  if (arg1 == arg2)
  {
    if (arg1 != this)
      M_contributors = arg1->M_contributors;
  }
  else if (arg1 == this || arg2 == this)
  {
    if (arg2 == this)
      std::swap(arg1, arg2);
    full_compare is_less;
    contributors_map::iterator i1 = M_contributors.begin();
    contributors_map::const_iterator i2 = arg2->M_contributors.begin();
    while (i1 != M_contributors.end() && i2 != arg2->M_contributors.end())
    {
      if (is_less(*i1, *i2))
	M_contributors.erase(i1++);
      else if (is_less(*i2, *i1))
	++i2;
      else
      {
	++i1;
	++i2;
      }
    }
    if (i1 != M_contributors.end())
      M_contributors.erase(i1, M_contributors.end());
  }
  else
  {
    M_contributors.clear();
    std::set_intersection(
	arg1->M_contributors.begin(),
	arg1->M_contributors.end(),
	arg2->M_contributors.begin(),
	arg2->M_contributors.end(),
	std::inserter(M_contributors, M_contributors.begin()),
	full_compare());
  }
  return *this;
}

template<>
ContributionsTxt& ContributionsTxt::operator=(ContributionsTxtOperator<ctop_union> const& args)
{
  typedef Inserter<contributors_map> insert_in;
  ContributionsTxt const* arg1 = &args.M_ct1;
  ContributionsTxt const* arg2 = &args.M_ct2;
  if (arg1 == arg2)
  {
    if (arg1 != this)
      M_contributors = arg1->M_contributors;
  }
  else if (arg1 == this)
    std::for_each(arg2->M_contributors.begin(),
	          arg2->M_contributors.end(),
		  insert_in(M_contributors));
  else if (arg2 == this)
    std::for_each(arg1->M_contributors.begin(),
	          arg1->M_contributors.end(),
		  insert_in(M_contributors));
  else
  {
    M_contributors.clear();
    std::set_union(
	arg2->M_contributors.begin(),
	arg2->M_contributors.end(),
	arg1->M_contributors.begin(),
	arg1->M_contributors.end(),
	std::inserter(M_contributors, M_contributors.begin()),
	contributors_map::key_compare());
  }
  return *this;
}

template<>
ContributionsTxt& ContributionsTxt::operator=(ContributionsTxtOperator<ctop_difference> const& args)
{
  ContributionsTxt const* arg1 = &args.M_ct1;
  ContributionsTxt const* arg2 = &args.M_ct2;
  if (arg1 == arg2)
    M_contributors.clear();
  else if (arg1 == this)
  {
    contributors_map::key_compare is_less;
    contributors_map::iterator i1 = M_contributors.begin();
    contributors_map::const_iterator i2 = arg2->M_contributors.begin();
    while (i1 != M_contributors.end() && i2 != arg2->M_contributors.end())
    {
      if (is_less(*i1, *i2))
	++i1;
      else if (is_less(*i2, *i1))
	++i2;
      else
	M_contributors.erase(i1++);
    }
  }
  else if (arg2 == this)
  {
    ContributionsTxt result;
    std::set_difference(
	arg1->M_contributors.begin(),
	arg1->M_contributors.end(),
	arg2->M_contributors.begin(),
	arg2->M_contributors.end(),
	std::inserter(result.M_contributors, result.M_contributors.begin()),
	contributors_map::key_compare());
    M_contributors = result.M_contributors;
  }
  else
  {
    M_contributors.clear();
    std::set_difference(
	arg1->M_contributors.begin(),
	arg1->M_contributors.end(),
	arg2->M_contributors.begin(),
	arg2->M_contributors.end(),
	std::inserter(M_contributors, M_contributors.begin()),
	contributors_map::key_compare());
  }
  return *this;
}

template<>
ContributionsTxt& ContributionsTxt::operator=(ContributionsTxtOperator<ctop_symmetric_difference> const& args)
{
  ContributionsTxt const* arg1 = &args.M_ct1;
  ContributionsTxt const* arg2 = &args.M_ct2;
  if (arg1 == arg2)
    M_contributors.clear();
  else if (arg1 == this || arg2 == this)
  {
    if (arg2 == this)
      std::swap(arg1, arg2);
    contributors_map::key_compare is_less;
    contributors_map::iterator i1 = M_contributors.begin();
    contributors_map::const_iterator i2 = arg2->M_contributors.begin();
    while (i1 != M_contributors.end() && i2 != arg2->M_contributors.end())
    {
      if (is_less(*i1, *i2))
	++i1;
      else if (is_less(*i2, *i1))
	M_contributors.insert(*i2++);
      else
      {
	M_contributors.erase(i1++);
	++i2;
      }
    }
    if (i2 != arg2->M_contributors.end())
      M_contributors.insert(i2, arg2->M_contributors.end());
  }
  else
  {
    M_contributors.clear();
    std::set_symmetric_difference(
	arg1->M_contributors.begin(),
	arg1->M_contributors.end(),
	arg2->M_contributors.begin(),
	arg2->M_contributors.end(),
	std::inserter(M_contributors, M_contributors.begin()),
	contributors_map::key_compare());
  }
  return *this;
}

