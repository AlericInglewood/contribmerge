// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file ContribGrammar.h This file contains the declaration of class ContribGrammar.
//
// Copyright (C) 2011, by Aleric Inglewood
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

#ifndef CONTRIBGRAMMAR_H
#define CONTRIBGRAMMAR_H

#ifndef USE_PCH
#include "sys.h"
#endif

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <iostream>
#include <string>

void print(int n)
{
  std::cout << "Got: " << n << std::endl;
}

void print_comment(std::vector<char> const& comment)
{
  std::cout << "Got: \"";
  for (std::vector<char>::const_iterator iter = comment.begin(); iter != comment.end(); ++iter)
  {
    std::cout << *iter;
  }
  std::cout << '"' << std::endl;
}

namespace grammar
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    template <typename Iterator>
    struct contributions_txt : qi::grammar<Iterator>
    {
        contributions_txt() : contributions_txt::base_type(file)
        {
	    using qi::alpha;
	    using qi::alnum;
	    using qi::blank;
	    using qi::eol;
            using qi::lit;
	    using qi::int_;
            using ascii::char_;

	    contributor_first_name =
	        alpha >> *alnum
	    ;

	    contributor_full_name =
	        contributor_first_name >> -(+blank >> +alpha)
	    ;

	    empty_line =
	    	*blank >> eol
	    ;

	    start =
	        empty_line >> contributor_full_name >> eol
	    ;

	    jira_project_key_prefix =
	    	(
		    /* Keys from https://jira.secondlife.com/secure/BrowseProjects.jspa#all
		     *
		     * Commented-out jira projects aren't present in
		     * doc/contributions.txt, yet and unlikely to ever
		     * get any entries there (business units, archived
		     * projects or unrelated to the Viewer)
		     */
		     
		    // Plublic Portals
		      lit("VWR")  // 1. Second Life Viewer - VWR
		    | lit("SVC")  // 2. Second Life Service - SVC
		    | lit("WEB")  // 3. Second Life Website - WEB
		    | lit("SEC")  // 4. Second Life Security Exploits - SEC
	//	    | lit("ARVD") // Archived
	//	    | lit("MISC") // Misc Issues (Archived)
		    | lit("SNOW") // Snowglobe
		    
		    // ECC
	//	    | lit("ECC")  // Business Unit: ECC
		    | lit("SH")   // Shining
		    
		    // LEAP
	//	    | lit("LEAP") // Business Unit: LEAP
		    | lit("DN")   // Display Names
		    
		    // Open Internal Projects
	//	    | lit("LLSD") // LLSD
		    | lit("CTS")  // Mesh Beta
	//	    | lit("PYO")  // Pyogp
		    | lit("STORM")// Snowstorm
	//	    | lit("TPV")  // Third-Party Viewers
		    
		    // Platform: Development
	//	    | lit("PLAT") // Business Unit: Platform
	//	    | lit("ER")   // Engine Room
		    
		    // JIRA projects that aren publicly accessible anymore
		    | lit("CT")   // Community Translations
		)
	    ;

	    jira_project_key =
		  (jira_project_key_prefix >> '-' >> int_)[&print]
		| lit("[NO JIRA]")
	    ;

	    any_char_but_eol =
	    	char_ - '\r' - '\n'
	    ;

	    comment =
	        (*(!(*blank >> eol) >> char_))[&print_comment]
	    ;

	    optional_comment =
	          (+blank >> comment >> *blank)
		| *blank
	    ;

	    contribution_entry =
	        +blank >> jira_project_key >> optional_comment >> eol
	    ;

	    contributor =
	    	contributor_full_name >> *blank >> eol >> *contribution_entry
	    ;

	    junk_line =
	        *any_char_but_eol >> eol
	    ;

	    leading_junk =
                *(junk_line - start)
            ;

	    file =
		qi::omit[leading_junk >> empty_line] >> +contributor >> qi::omit[*empty_line]
	    ;
        }

        qi::rule<Iterator> contributor_first_name;
        qi::rule<Iterator> contributor_full_name;
        qi::rule<Iterator> empty_line;
        qi::rule<Iterator> start;
        qi::rule<Iterator> any_char_but_eol;
        qi::rule<Iterator> junk_line;
        qi::rule<Iterator> leading_junk;
	qi::rule<Iterator> jira_project_key_prefix;
	qi::rule<Iterator> jira_project_key;
	qi::rule<Iterator> comment;
	qi::rule<Iterator> optional_comment;
	qi::rule<Iterator> contribution_entry;
	qi::rule<Iterator> contributor;
        qi::rule<Iterator> file;
    };

} // namespace grammar

#endif	// CONTRIBGRAMMAR_H
