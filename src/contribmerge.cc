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

#include <algorithm>
#include <cassert>
#include <boost/program_options.hpp>
#include "contribmerge.h"
#include "ContributionsTxt.h"
#include "exceptions.h"
#include "three_way_merge.h"

struct CommentEqual
{
  bool operator()(ContributionEntry const& ce1, ContributionEntry const& ce2)
  {
    assert(!(ce1.jira_project_key() < ce2.jira_project_key()) && !(ce2.jira_project_key() < ce1.jira_project_key()));
    return ce1.comment() == ce2.comment();
  }
};

// We can't do this, so fail -- but lets print an error message that explains what's wrong.
struct CommentMerger
{
  template<typename Iterator1, typename Iterator2, typename Iterator3, typename OutputIterator>
  void operator()(Iterator1 l, Iterator2 b, Iterator3 r, OutputIterator&) throw(MergeFailure)
  {
    if (l == Iterator1())
    {
      std::cerr << "Failure: hit rule: n (-, m) --> MergeFailure.\n";
      std::cerr << "With n = <ContributionEntry>" << b->jira_project_key() << b->comment() <<
	  "</ContributionEntry> and m = <ContributionEntry>" << r->jira_project_key() << r->comment() <<
	  "</ContributionEntry>" << std::endl;
      throw MergeFailure();
    }
    else if (r == Iterator3())
    {
      std::cerr << "Failure: hit rule: n (m, -) --> MergeFailure.\n";
      std::cerr << "With n = <ContributionEntry>" << b->jira_project_key() << b->comment() <<
	  "</ContributionEntry> and m = <ContributionEntry>" << l->jira_project_key() << l->comment() <<
	  "</ContributionEntry>" << std::endl;
      throw MergeFailure();
    }
    else if (b == Iterator2())
    {
      std::cerr << "Failure: hit rule: - (n, m) --> MergeFailure.\n";
      std::cerr << "With n = <ContributionEntry>" << l->jira_project_key() << l->comment() <<
	  "</ContributionEntry> and m = <ContributionEntry>" << r->jira_project_key() << r->comment() <<
	  "</ContributionEntry>" << std::endl;
      throw MergeFailure();
    }
    else
    {
      std::cerr << "Failure: hit rule: n (m, k) --> MergeFailure.\n";
      std::cerr << "With n = <ContributionEntry>" << b->jira_project_key() << b->comment() <<
	  "</ContributionEntry> and m = <ContributionEntry>" << l->jira_project_key() << l->comment() <<
	  "</ContributionEntry> and k = <ContributionEntry>" << r->jira_project_key() << r->comment() <<
	  "</ContributionEntry>" << std::endl;
      throw MergeFailure();
    }
  }
};

struct ContributionsEqual
{
  bool operator()(Contributor const& c1, Contributor const& c2)
  {
    assert(c1.first == c2.first);
    return c1.second.raw_string() == c2.second.raw_string();
  }
};

struct ContributionsMerger
{
  template<typename Iterator1, typename Iterator2, typename Iterator3, typename OutputIterator>
  void operator()(Iterator1 l, Iterator2 b, Iterator3 r, OutputIterator& output) throw(MergeFailure)
  {
    if (l == Iterator1())
    {
      std::cerr << "Failure: hit rule: n (-, m) --> MergeFailure.\n";
      std::cerr << "With n = <RAW>" << b->second.raw_string() << "</RAW> and m = <RAW>" << r->second.raw_string() << "</RAW>" << std::endl;
      throw MergeFailure();
    }
    else if (r == Iterator3())
    {
      std::cerr << "Failure: hit rule: n (m, -) --> MergeFailure.\n";
      std::cerr << "With n = <RAW>" << b->second.raw_string() << "</RAW> and m = <RAW>" << l->second.raw_string() << "</RAW>" << std::endl;
      throw MergeFailure();
    }
    else if (b == Iterator2())
    {
      assert(l->first == r->first);
      FormattedContributions left(l->second), right(r->second);
      FormattedContributions result;

      std::set_union(left.contributions().begin(), left.contributions().end(),
		     right.contributions().begin(), right.contributions().end(),
		     result.get_inserter(),
		     FormattedContributions::contributions_type::key_compare());

      *output = Contributor(l->first, result);
    }
    else
    {
      assert(l->first == r->first);
      FormattedContributions left(l->second), base(b->second), right(r->second);
      FormattedContributions result;

      three_way_merge(left.contributions().begin(), left.contributions().end(),
		      base.contributions().begin(), base.contributions().end(),
		      right.contributions().begin(), right.contributions().end(),
		      result.get_inserter(),
		      CommentMerger(),
		      FormattedContributions::contributions_type::key_compare(),
		      CommentEqual());

      *output = Contributor(b->first, result);
    }
    ++output;
  }
};

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

  three_way_merge(left.contributors().begin(), left.contributors().end(),
		  base.contributors().begin(), base.contributors().end(),
		  right.contributors().begin(), right.contributors().end(),
		  result.get_inserter(), ContributionsMerger(),
		  ContributionsTxt::contributors_map::key_compare(),
		  ContributionsEqual());

  return result;
}

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
  Debug(debug::init());

  std::string filename_base;
  std::string filename_left;
  std::string filename_right;
//  std::string filename_out;

  po::options_description visible_options("Allowed options");
  visible_options.add_options()
    ("help,h", "Produce help message.")
//    ("p", "Send results to standard output instead of overwriting file 'left'.")
//    ("quiet,q", "Quiet; do not warn about conflicts.")
    ("version,V", "Print version number.")
//    ("out,o", po::value<std::string>(&filename_out), "Out file. If passed, the merge result will be written there in case of a successful merge.")
  ;

  // Separate descriptions for positional options, so they don't show up in help.
  po::options_description hidden_options;
  hidden_options.add_options()
    ("base", po::value<std::string>(&filename_base), "contributions.txt in the youngest common ancestor of the two merged revisions.")
    ("left", po::value<std::string>(&filename_left), "contributions.txt in one of the merged revisions.")
    ("right", po::value<std::string>(&filename_right), "contributions.txt in the other merged revision.")
  ;

  po::options_description cmdline_options;
  cmdline_options.add(visible_options).add(hidden_options);

  po::positional_options_description p;
  p.add("base", 1)
   .add("left", 1)
   .add("right", 1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(cmdline_options)
                                               .positional(p).run(),
            vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    std::cout << visible_options << std::endl;
    return 1;
  }

  if (vm.count("version"))
  {
#ifdef PACKAGE_STRING //! \TODO Remove ifdef once config.h is generated by CMake, too.
    std::cout << PACKAGE_STRING << std::endl;
#else
    std::cout << "contribmerge (version unknown)" << std::endl;
#endif
    return 1;
  }

  try
  {
    ContributionsTxt base(filename_base);
    ContributionsTxt left(filename_left);
    ContributionsTxt right(filename_right);

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

