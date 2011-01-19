#include <iostream>
#include <string>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

// Rule: jira_project_key
struct JiraProjectKey {
  std::string jira_project_key_prefix;
  int issue_number;
};

std::ostream& operator<<(std::ostream& os, JiraProjectKey const& key)
{
  return os << key.jira_project_key_prefix << '-' << key.issue_number;
}

// Rule: contribution_entry
// Input: "\tVWR-101 (optional comment) \n" or "   SNOW-102  \n" (no comment).
struct ContributionEntry {
  JiraProjectKey jira_project_key;
  std::string comment;				// Optional (empty if there is none).
};

std::ostream& operator<<(std::ostream& os, ContributionEntry const& entry)
{
  os << entry.jira_project_key;
  if (!entry.comment.empty())
    os << ' ' << entry.comment;
  return os;
}

// Rule: contributor
struct Contributor {
  std::string full_name;
  std::vector<ContributionEntry> contributions;
};

std::ostream& operator<<(std::ostream& os, Contributor const& contributor)
{
  os << contributor.full_name << "\\n\n";
  for (std::vector<ContributionEntry>::const_iterator iter = contributor.contributions.begin(); iter != contributor.contributions.end(); ++iter)
  {
    os << '\t' << *iter << "\\n\n";
  }
  return os;
}

// The type I want to parse the data into:
struct ContributionsTxt {
  std::string header;
  std::vector<Contributor> contributors;
};

BOOST_FUSION_ADAPT_STRUCT(
    JiraProjectKey,
    (std::string, jira_project_key_prefix)
    (int, issue_number)
)

BOOST_FUSION_ADAPT_STRUCT(
    ContributionEntry,
    (JiraProjectKey, jira_project_key)
    (std::string, comment)
)

BOOST_FUSION_ADAPT_STRUCT(
    Contributor,
    (std::string, full_name)
    (std::vector<ContributionEntry>, contributions)
)

BOOST_FUSION_ADAPT_STRUCT(
    ContributionsTxt,
    (std::string, header)
    (std::vector<Contributor>, contributors)
)
  
void print_int(int n)
{
  std::cout << "Got: " << n << "\n";
}

void print_test(std::string const& str)
{
  std::cout << "Got: \"" << str << "\".\n";
}

void print_test2(std::string const& str)
{
  std::cout << "contribution_entry: \"" << str << "\".\n";
}

namespace grammar
{
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
      using qi::int_;
      using qi::omit;
      using qi::raw;
      using qi::string;
      using ascii::char_;

      contributor_first_name =
	  alpha >> *alnum
      ;

      contributor_last_name =
	  &(+blank >> alpha) >> omit[+blank] >> qi::attr(' ') >> +alpha
	  //-(omit[+blank] >> qi::attr(' ') >> +alpha)
      ;

      contributor_full_name =
	  contributor_first_name >> (contributor_last_name | qi::eps)
	  // The raw[] is needed for boost < 1.46.
	  // Unfortunately this causes blanks between first and last to be returned literally.
	  //contributor_first_name >> raw[contributor_last_name];
      ;

      empty_line =
	  *blank >> eol
      ;

      start =
	  empty_line >> contributor_full_name >> *blank >> eol
      ;

      jira_project_key_prefix =
	  (
		string("VWR")  // 1. Second Life Viewer - VWR
	      | string("SVC")  // 2. Second Life Service - SVC
	      | string("WEB")  // 3. Second Life Website - WEB
	      | string("SEC")  // 4. Second Life Security Exploits - SEC
	      | string("SNOW") // Snowglobe
	      | string("SH")   // Shining
	      | string("DN")   // Display Names
	      | string("CTS")  // Mesh Beta
	      | string("STORM")// Snowstorm
	      | string("CT")   // Community Translations
	  )
      ;

      jira_project_key =
	    ("[NO JIRA]" >> qi::attr(0))
	  | (jira_project_key_prefix[&print_test] >> '-' >> int_[&print_int])
      ;

      any_char_but_eol =
	  char_ - '\r' - '\n'
      ;

      comment =
	  *(!(*blank >> eol) >> char_)
      ;

      optional_comment =
	    (omit[+blank] >> comment >> omit[*blank])
	  | omit[*blank]
      ;

      contribution_entry =
	  omit[+blank] >> jira_project_key >> optional_comment >> eol
      ;

      contributor =
	  contributor_full_name >> omit[*blank] >> eol >> *contribution_entry
      ;

      header_line =
	  *any_char_but_eol >> eol
      ;

      header =
	  // Return the header literally.
	  raw[*(header_line - start)]
      ;

      contributions_txt =
	  header >> empty_line >> +contributor >> *empty_line
      ;
    }

    qi::rule<Iterator, std::string()> contributor_first_name;
    qi::rule<Iterator, std::string()> contributor_last_name;
    qi::rule<Iterator, std::string()> contributor_full_name;
    qi::rule<Iterator> empty_line;
    qi::rule<Iterator> start;
    qi::rule<Iterator, char()> any_char_but_eol;
    qi::rule<Iterator, std::string()> header_line;
    qi::rule<Iterator, std::string()> header;
    qi::rule<Iterator, std::string()> jira_project_key_prefix;
    qi::rule<Iterator, JiraProjectKey()> jira_project_key;
    qi::rule<Iterator, std::string()> comment;
    qi::rule<Iterator, std::string()> optional_comment;
    qi::rule<Iterator, ContributionEntry()> contribution_entry;
    qi::rule<Iterator, Contributor()> contributor;
    qi::rule<Iterator, ContributionsTxt()> contributions_txt;
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
"First123name    Lastname   \n"
  "\tVWR-101\n"
"    SNOW-102 (some comment)  \n"
 "\t STORM-103\n"

"JustFirstname\n"
  "\tVWR-201\n"
  "\t[NO JIRA] (but worked hard on this)\n"
  "\tSTORM-203  \n"

"JustFirstnameWith3Space   \n"
   " STORM-301  \n"

// Trailing white spaces.
"   \n\n";

int main()
{
  typedef std::string::const_iterator iterator_type;
  typedef grammar::contributions_txt_grammar<iterator_type> contributions_txt_grammar;

  iterator_type iter = input.begin();
  iterator_type const end = input.end();
  contributions_txt_grammar contributions_txt_parser;

  ContributionsTxt result;
  bool r = parse(iter, end, contributions_txt_parser, result);

  if (r && iter == end)
  {
    std::cout << "Parsing succeeded\n";
    std::cout << "Header:=======================================\n" << result.header << "==============================================\n";
    std::cout << "Number of Contributors: " << result.contributors.size() << std::endl;
    for (std::vector<Contributor>::iterator iter = result.contributors.begin(); iter != result.contributors.end(); ++iter)
    {
      std::cout << *iter;
    }
  }
  else
  {
    std::string rest(iter, end);
    std::cout << "Parsing failed\n" << "stopped at: \": " << rest << "\"\n";
  }
}

