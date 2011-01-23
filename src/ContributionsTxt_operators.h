// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file ContributionsTxt_operators.h Implementation of operators of ContributionsTxt.
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

#ifndef CONTRIBUTIONSTXT_OPERATORS_H
#define CONTRIBUTIONSTXT_OPERATORS_H

#include "ContributionsTxt.h"
#include <algorithm>

inline bool operator==(ContributionsTxt const& ct1, ContributionsTxt const& ct2)
{
  return ct1.header() == ct2.header();
}

inline bool operator!=(ContributionsTxt const& ct1, ContributionsTxt const& ct2)
{
  return ct1.header() != ct2.header();
}

enum ctop_types {
  ctop_or,
  ctop_and,
  ctop_union,
  ctop_difference,
  ctop_symmetric_difference
};

template<int ctop>
struct ContributionsTxtOperator
{
  ContributionsTxt const& M_ct1;
  ContributionsTxt const& M_ct2;
  ContributionsTxtOperator(ContributionsTxt const& ct1, ContributionsTxt const& ct2) : M_ct1(ct1), M_ct2(ct2) { }
};

template<class Container>
struct Inserter {
  std::insert_iterator<Container> M_inserter;
  Inserter(Container& container) : M_inserter(std::inserter(container, container.begin())) { }
  void operator()(typename Container::value_type const& value) { *M_inserter = value; }
};

inline ContributionsTxtOperator<ctop_or> operator|(ContributionsTxt const& ct1, ContributionsTxt const& ct2)
{
  return ContributionsTxtOperator<ctop_or>(ct1, ct2);
}

inline ContributionsTxtOperator<ctop_and> operator&(ContributionsTxt const& ct1, ContributionsTxt const& ct2)
{
  return ContributionsTxtOperator<ctop_and>(ct1, ct2);
}

inline ContributionsTxtOperator<ctop_union> operator+(ContributionsTxt const& ct1, ContributionsTxt const& ct2)
{
  return ContributionsTxtOperator<ctop_union>(ct1, ct2);
}

inline ContributionsTxtOperator<ctop_difference> operator-(ContributionsTxt const& ct1, ContributionsTxt const& ct2)
{
  return ContributionsTxtOperator<ctop_difference>(ct1, ct2);
}

inline ContributionsTxtOperator<ctop_symmetric_difference> operator^(ContributionsTxt const& ct1, ContributionsTxt const& ct2)
{
  return ContributionsTxtOperator<ctop_symmetric_difference>(ct1, ct2);
}

template<>
ContributionsTxt& ContributionsTxt::operator=(ContributionsTxtOperator<ctop_or> const&)
{
  return *this;
}

template<>
ContributionsTxt& ContributionsTxt::operator=(ContributionsTxtOperator<ctop_and> const&)
{
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

#endif // CONTRIBUTIONSTXT_OPERATORS_H
