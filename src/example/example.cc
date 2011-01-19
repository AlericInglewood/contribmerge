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
	  contributor_first_name >> (contributor_last_name | qi::eps)
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

      // A jira project key.
      // The attribute is JiraProjectKey.
      jira_project_key =
	    ("[NO JIRA]" >> qi::attr(0))
	  | (jira_project_key_prefix[&print_test] >> '-' >> int_[&print_int])
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
  "\t[NO JIRA]\t\t(but worked hard on this)\n"
  "\tSTORM-203  \n"

"JustFirstnameWith3Spaces   \n"
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

