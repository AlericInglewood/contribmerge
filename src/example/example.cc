#include <iostream>
#include <string>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

// Rule: contribution_entry
// Input: "\tVWR-101 (optional comment) \n" or "   SNOW-102  \n" (no comment).
struct ContributionEntry {
  std::string jira_project_key_prefix;		// Ie, "VWR"
  int issue_number;				// Ie, 101
  std::string comment;				// Optional (empty if there is none).
};

// Rule: contributor
struct Contributor {
  std::string full_name;
  std::vector<ContributionEntry> contributions;
};

// The type I want to parse the data into:
struct WholeFile {
  std::string header;
  std::vector<Contributor> contributors;
};

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
		lit("VWR")  // 1. Second Life Viewer - VWR
	      | lit("SVC")  // 2. Second Life Service - SVC
	      | lit("WEB")  // 3. Second Life Website - WEB
	      | lit("SEC")  // 4. Second Life Security Exploits - SEC
	      | lit("SNOW") // Snowglobe
	      | lit("SH")   // Shining
	      | lit("DN")   // Display Names
	      | lit("CTS")  // Mesh Beta
	      | lit("STORM")// Snowstorm
	      | lit("CT")   // Community Translations
	  )
      ;

      jira_project_key =
	    (jira_project_key_prefix >> '-' >> int_)
	  | lit("[NO JIRA]")
      ;

      any_char_but_eol =
	  char_ - '\r' - '\n'
      ;

      comment =
	  *(!(*blank >> eol) >> char_)
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

      header_line =
	  *any_char_but_eol >> eol
      ;

      header =
	  *(header_line - start)
      ;

      file =
	  header >> empty_line >> +contributor >> qi::omit[*empty_line]
      ;
    }

    qi::rule<Iterator> contributor_first_name;
    qi::rule<Iterator> contributor_full_name;
    qi::rule<Iterator> empty_line;
    qi::rule<Iterator> start;
    qi::rule<Iterator> any_char_but_eol;
    qi::rule<Iterator> header_line;
    qi::rule<Iterator> header;
    qi::rule<Iterator> jira_project_key_prefix;
    qi::rule<Iterator> jira_project_key;
    qi::rule<Iterator> comment;
    qi::rule<Iterator> optional_comment;
    qi::rule<Iterator> contribution_entry;
    qi::rule<Iterator> contributor;
    qi::rule<Iterator> file;
  };

} // namespace grammar

std::string const input =
// Header
"header header header  \n"
"more header\n"
"\n"
"still more header\n"
"\n"	// Empty line
// First contributor starts here
"Firstname1 Lastname\n"
  "\tVWR-101\n"
"    SNOW-102 (some comment)  \n"
 "\t STORM-103\n"

"Firstname2\n"
  "\tVWR-201\n"
  "\t[NO JIRA] (but worked hard on this)\n"
  "\tSTORM-203  \n"

"Firstname3 Lastname\n"
   " STORM-301  \n"

// Trailing white spaces.
"   \n\n";

int main()
{
    typedef std::string::const_iterator iterator_type;
    typedef grammar::contributions_txt<iterator_type> contributions_txt;

    iterator_type iter = input.begin();
    iterator_type const end = input.end();
    contributions_txt contributions_txt_parser;

    WholeFile result;
    bool r = parse(iter, end, contributions_txt_parser, result);

    if (r && iter == end)
    {
	std::cout << "Parsing succeeded\n";
	std::cout << "Header: \"" << result.header << "\".\n";
	std::cout << "Contributors: " << result.contributors.size() << std::endl;
    }
    else
    {
	std::string rest(iter, end);
	std::cout << "Parsing failed\n" << "stopped at: \": " << rest << "\"\n";
    }
}

