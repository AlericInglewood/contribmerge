#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>

template<typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& v)
{
  os << '{';
  bool first = true;
  for (typename std::vector<T>::const_iterator iter = v.begin(); iter !=  v.end(); ++iter)
  {
    if (first)
      first = false;
    else
      std::cout << ", ";
    std::cout << *iter;
  }
  os << '}';
  return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, std::set<T> const& s)
{
  os << '{';
  bool first = true;
  for (typename std::set<T>::const_iterator iter = s.begin(); iter !=  s.end(); ++iter)
  {
    if (first)
      first = false;
    else
      std::cout << ", ";
    std::cout << *iter;
  }
  os << '}';
  return os;
}

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, std::pair<T1, T2> const& p)
{
  return os << '{' << p.first << ", " << p.second << '}';
}

template<typename K, typename V, typename P, typename A>
std::ostream& operator<<(std::ostream& os, std::map<K, V, P, A> const& m)
{
  os << '{';
  bool first = true;
  for (typename std::map<K, V, P, A>::const_iterator iter = m.begin(); iter !=  m.end(); ++iter)
  {
    if (first)
      first = false;
    else
      std::cout << ", ";
    std::cout << *iter;
  }
  os << '}';
  return os;
}

struct Pred {
  bool operator()(std::pair<int const, char> const& a, std::pair<int const, char> const& b) const { return a.first < b.first; }
};

int main()
{
  using namespace boost::spirit;
  using namespace boost::spirit::qi;
  using namespace boost::phoenix;

  typedef char const* Iterator;
  Iterator data[2] = {
      "1A    3C 4D 5E",
      "1a 2b    4d     6f"
  };

  typedef std::map<int, char> Result;

  uint_parser<int, 10, 1, 1> one_digit;
  rule<Iterator, std::pair<int, char>()> pair = one_digit >> char_("a-fA-F");
  rule<Iterator, Result()> grammar = pair % +blank;

  Result result[3];
  for (int i = 0; i < 2; ++i)
  {
    Iterator first = data[i];
    Iterator const last = data[i] + std::strlen(data[i]);
    bool success = parse(first, last, grammar, result[i]);
    if (success)
      std::cout << "Input " << i << ":      " << result[i] << std::endl;
    else
      std::cout << "parse error" << std::endl;
  }

  Result output;
  std::set_difference(result[0].begin(), result[0].end(), result[1].begin(), result[1].end(), std::inserter(output, output.begin()), Pred());
  std::cout << "Difference:   " << output << std::endl;
  output.clear();
  std::set_intersection(result[0].begin(), result[0].end(), result[1].begin(), result[1].end(), std::inserter(output, output.begin()), Pred());
  std::cout << "Intersection: " << output << std::endl;
  output.clear();
  std::set_symmetric_difference(result[0].begin(), result[0].end(), result[1].begin(), result[1].end(), std::inserter(output, output.begin()), Pred());
  std::cout << "Sym. diff:    " << output << std::endl;
  output.clear();
  std::set_union(result[0].begin(), result[0].end(), result[1].begin(), result[1].end(), std::inserter(output, output.begin()), Pred());
  std::cout << "Union(0,1):   " << output << std::endl;
  output.clear();
  std::set_union(result[1].begin(), result[1].end(), result[0].begin(), result[0].end(), std::inserter(output, output.begin()), Pred());
  std::cout << "Union(1,0):   " << output << std::endl;

  return 0;
}
