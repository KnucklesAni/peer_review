#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

class DomainChecker;
class Domain {
public:
  Domain(string_view name) : reversed_name_(ReversedName(name)) {}
  bool operator==(const Domain &other) const {
    return reversed_name_ == other.reversed_name_;
  }
  bool IsSubdomain(const Domain &other) const {
    if (other.reversed_name_.length() > reversed_name_.length()) {
      return false;
    }
    string_view sub_part{&reversed_name_.front(),
                         other.reversed_name_.length()};
    return other.reversed_name_ == sub_part;
  }

private:
  string reversed_name_;
  static string ReversedName(string_view name) {
    string reversed_name{name};
    reverse(reversed_name.begin(), reversed_name.end());
    reversed_name.push_back('.');
    return reversed_name;
  }
  friend class DomainChecker;
};

class DomainChecker {
public:
  template <typename ItBegin, typename ItEnd>
  DomainChecker(ItBegin it_begin, ItEnd it_end) : domains_(it_begin, it_end) {
    sort(domains_.begin(), domains_.end(), Comparator);
    domains_.erase(
        unique(domains_.begin(), domains_.end(),
               [](Domain &lhs, Domain &rhs) { return rhs.IsSubdomain(lhs); }),
        domains_.end());
  }
  bool IsForbidden(const Domain &other) const {
    auto it = upper_bound(domains_.begin(), domains_.end(), other, Comparator);
    if (it == domains_.begin()) {
      return false;
    }
    it--;
    return other.IsSubdomain(*it);
  }

private:
  vector<Domain> domains_;
  static constexpr auto Comparator = [](const Domain &lhs, const Domain &rhs) {
    return lexicographical_compare(
        lhs.reversed_name_.begin(), lhs.reversed_name_.end(),
        rhs.reversed_name_.begin(), rhs.reversed_name_.end());
  };
};
vector<Domain> ReadDomains(istream &input, size_t N) {
  vector<Domain> result;
  for (size_t i = 0; i < N; ++i) {
    string line;
    getline(input, line);
    result.push_back(Domain{line});
  }
  return result;
}

template <typename Number> Number ReadNumberOnLine(istream &input) {
  string line;
  getline(input, line);

  Number num;
  std::istringstream(line) >> num;

  return num;
}

int main() {
  const std::vector<Domain> forbidden_domains =
      ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
  DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

  const std::vector<Domain> test_domains =
      ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
  for (const Domain &domain : test_domains) {
    cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
  }
}