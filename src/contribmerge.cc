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

#include <string>
#include "contribmerge.h"
#include "ContributionsTxt.h"
#include "ContributionsTxt_operators.h"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
  Debug(debug::init());

  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("base", po::value< std::string >(), "contributions.txt in the youngest common ancestor of the two merged revisions")
    ("left", po::value< std::string >(), "contributions.txt in one of the merged revisions" )
    ("right", po::value< std::string >(), "contributions.txt in the other merged revision" )
    ("out,o", po::value< std::string >(), "out file. If passed, the merge result will be written there in case of a successful merge." )
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 1;
  }

  ContributionsTxt base;
  ContributionsTxt left;
  ContributionsTxt right;
  try
  {
    base.parse("base");
    left.parse("left");
    right.parse("right");
    std::cout << "Parsing succeeded\n";
  }
  catch(ParseError& parse_error)
  {
    std::cout << "Parsing failed\n" << "Stopped at: \"" << parse_error.rest() << "\"\n";
    exit(1);
  }

  return 0;
}

