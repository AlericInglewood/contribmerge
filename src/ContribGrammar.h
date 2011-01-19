// contribmerge -- A three-way merge utility for doc/contributions.txt
//
//! @file ContribGrammar.h This file contains the declaration of class ContribGrammar.
//
// Copyright (C) 2011, by Aleric Inglewood & Boroondas Gupte
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
#include <boost/fusion/include/adapt_struct.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace attributes {

// Rule: jira_project_key
struct JiraProjectKey {
  std::string jira_project_key_prefix;		// "VWR"
  int issue_number;				// 123
};

// Rule: contribution_entry
struct ContributionEntry {
  JiraProjectKey jira_project_key;		// "VWR-123"
  std::string comment;				// Optional (empty if there is none).
};

// Rule: contributor
struct Contributor {
  std::string full_name;			// "Firstname[ Lastname]"
  std::vector<ContributionEntry> contributions;
};

// The type I want to parse the data into:
struct ContributionsTxt {
  std::string header;				// Raw header text.
  std::vector<Contributor> contributors;
};

} // namespace attributes

BOOST_FUSION_ADAPT_STRUCT(
    attributes::JiraProjectKey,
    (std::string, jira_project_key_prefix)
    (int, issue_number)
)

BOOST_FUSION_ADAPT_STRUCT(
    attributes::ContributionEntry,
    (attributes::JiraProjectKey, jira_project_key)
    (std::string, comment)
)

BOOST_FUSION_ADAPT_STRUCT(
    attributes::Contributor,
    (std::string, full_name)
    (std::vector<attributes::ContributionEntry>, contributions)
)

BOOST_FUSION_ADAPT_STRUCT(
    attributes::ContributionsTxt,
    (std::string, header)
    (std::vector<attributes::Contributor>, contributors)
)

namespace grammar
{
  using namespace attributes;

  namespace qi = boost::spirit::qi;
  namespace fusion = boost::fusion;
  namespace ascii = boost::spirit::ascii;

  template <typename Iterator>
  struct contributions_txt_grammar : qi::grammar<Iterator, ContributionsTxt()>
  {
    contributions_txt_grammar() : contributions_txt_grammar::base_type(contributions_txt)
    {
      using qi::alpha;
      using qi::alnum;
      using qi::blank;
      using qi::eol;
      using qi::lit;
      using qi::eps;
      using qi::int_;
      using qi::omit;
      using qi::raw;
      using qi::string;
      using ascii::char_;

      // The first name of a contributor.
      // Attribute: std::string.
      contributor_first_name =
	  alpha >> *alnum
      ;

      // Whitespace plus last name of a contributor (optionally follows contributor_first_name).
      // Attribute: std::string.
      contributor_last_name =
	  // The &(+blank >> alpha) stops this rule from adding a trailing space
	  // to the attribute, only to find out that it fails because there is
	  // no last name (the attribute is not reset in that case).
	  &(+blank >> alpha) >>
	  // At least one blank (translated into a single space), followed by alpha characters.
	  omit[+blank] >> qi::attr(' ') >> +alpha
      ;

      // The full name of a contributor. If there is no last name, then the assumption has
      // to be that it is "FirstName Resident".
      // Attribute: std::string.
      contributor_full_name =
	  // The trick with eps is to simulate 'optional'. Using a real
	  // optional doesn't work here because of a bug in boost::spirit
	  // (it would cause the attribute to be reset).
	  // This should be fixed in boost >= 1.46.
	  contributor_first_name >> (contributor_last_name | eps)
      ;

      // An eol of sequence including optional preceding white space.
      // Trailing white space is always condensed into a single eol on output, hence it has no attribute.
      newline =
	  *blank >> eol
      ;

      // A line existing of (optionally) only blanks, including the eol.
      // Empty lines are always condensed into a single eol on output, hence it has no attribute.
      empty_line =
	  newline
      ;

      // Helper rule to find the start of the contributors list.
      // This rule has no attribute.
      start =
	  empty_line >> contributor_full_name >> newline;
      ;

      // A jira project key prefix.
      // Attribute: std::string.
      jira_project_key_prefix =
	  (
	      // Keys from https://jira.secondlife.com/secure/BrowseProjects.jspa#all
	      //
	      // Commented-out jira projects aren't present in
	      // doc/contributions.txt, yet and unlikely to ever
	      // get any entries there (business units, archived
	      // projects or unrelated to the Viewer)
	       
	      // Plublic Portals
		string("VWR")  // 1. Second Life Viewer - VWR
	      | string("SVC")  // 2. Second Life Service - SVC
	      | string("WEB")  // 3. Second Life Website - WEB
	      | string("SEC")  // 4. Second Life Security Exploits - SEC
//	      | string("ARVD") // Archived
//	      | string("MISC") // Misc Issues (Archived)
	      | string("SNOW") // Snowglobe
	      
	      // ECC
//	      | string("ECC")  // Business Unit: ECC
	      | string("SH")   // Shining
	      
	      // LEAP
//	      | string("LEAP") // Business Unit: LEAP
	      | string("DN")   // Display Names
	      
	      // Open Internal Projects
//	      | string("LLSD") // LLSD
	      | string("CTS")  // Mesh Beta
//	      | string("PYO")  // Pyogp
	      | string("STORM")// Snowstorm
//	      | string("TPV")  // Third-Party Viewers
	      
	      // Platform: Development
//	      | string("PLAT") // Business Unit: Platform
//	      | string("ER")   // Engine Room
	      
	      // JIRA projects that aren publicly accessible anymore
	      | string("CT")   // Community Translations
	  )
      ;

      // A jira project key.
      // The attribute is JiraProjectKey.
      jira_project_key =
	    (jira_project_key_prefix >> '-' >> int_)
	  | (string("[NO JIRA]") >> (int_ | eps))
      ;

      // Any character except eol characters.
      // Attribute: char.
      any_char_but_eol =
	  char_ - '\r' - '\n'
      ;

      // A space trimmed comment.
      // Attribute: std::string.
      comment =
	  // The !newline causes trailing white space not to be included,
	  // and to terminate at the end of a line.
	  *(!newline >> char_)
      ;

      // Mandatory blank(s) followed by a jira project key, followed by a comment (that may be empty).
      // Attribute: ContributionEntry.
      contribution_entry =
	  omit[+blank] >> jira_project_key >> comment >> newline;
      ;

      // Name of a contributor, followed by a list of their contributions.
      // Attribute: Contributor.
      contributor =
	  contributor_full_name >> newline >> *contribution_entry
      ;

      // As an exception, the header line preserves trailing whitespace.
      // Attribute: std::string.
      header_line =
	  *any_char_but_eol >> eol
      ;

      // The complete header, exclusive the final empty line.
      // This is a literal string containing newlines and trailing whitespaces.
      // Attribute: std::string.
      header =
	  // Return the header literally. Stop when we see 'start'.
	  raw[*(header_line - start)]
      ;

      // Attribute: ContributionsTxt.
      contributions_txt =
	  header >> empty_line >> +contributor >> *empty_line
      ;
    }

    qi::rule<Iterator, std::string()> contributor_first_name;
    qi::rule<Iterator, std::string()> contributor_last_name;
    qi::rule<Iterator, std::string()> contributor_full_name;
    qi::rule<Iterator> newline;
    qi::rule<Iterator> empty_line;
    qi::rule<Iterator> start;
    qi::rule<Iterator, char()> any_char_but_eol;
    qi::rule<Iterator, std::string()> header_line;
    qi::rule<Iterator, std::string()> header;
    qi::rule<Iterator, std::string()> jira_project_key_prefix;
    qi::rule<Iterator, JiraProjectKey()> jira_project_key;
    qi::rule<Iterator, std::string()> comment;
    qi::rule<Iterator, ContributionEntry()> contribution_entry;
    qi::rule<Iterator, Contributor()> contributor;
    qi::rule<Iterator, ContributionsTxt()> contributions_txt;
  };

} // namespace grammar

#endif	// CONTRIBGRAMMAR_H
