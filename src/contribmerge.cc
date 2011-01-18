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
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include "debug.h"
#endif

#include "contribmerge.h"

int main()
{
    Debug(debug::init());

    std::ifstream infile;
    infile.open("testinput");
    infile.unsetf(std::ios::skipws);

    typedef boost::spirit::istream_iterator iterator_type;
    typedef grammar::contributions_txt<iterator_type> contributions_txt;

    contributions_txt contributions_txt_parser; // Our grammar

    std::string str;
    unsigned result;

    boost::spirit::istream_iterator iter(infile);
    boost::spirit::istream_iterator end;

    bool r = parse(iter, end, contributions_txt_parser, result);

    if (r && iter == end)
    {
	std::cout << "-------------------------\n";
	std::cout << "Parsing succeeded\n";
	std::cout << "result = " << result << std::endl;
	std::cout << "-------------------------\n";
    }
    else
    {
	std::string rest(iter, end);
	std::cout << "-------------------------\n";
	std::cout << "Parsing failed\n";
	std::cout << "stopped at: \": " << rest << "\"\n";
	std::cout << "-------------------------\n";
    }

    infile.close();

    std::cout << "Bye... :-) \n\n";
    return 0;
}

