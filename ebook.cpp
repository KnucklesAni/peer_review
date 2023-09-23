#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

int main() {
  auto pages_to_users = new int[10000]();
  auto user_to_pages = new int[10000000]();
  int total_users = 0;
  int N;
  cin >> N;
  while (N-- > 0) {
    string operation;
    cin >> operation;
    if (operation == "READ") {
      int id, pages;
      cin >> id;
      cin >> pages;
      for (int page = user_to_pages[id] + 1; page <= pages; page++) {
        pages_to_users[page]++;
      }
      total_users += user_to_pages[id] == 0;
      user_to_pages[id] = pages;
    } else if (operation == "CHEER") {
      int id;
      cin >> id;
      if (user_to_pages[id] == 0) {
        cout << "0\n";
      } else if (total_users == 1) {
        cout << "1\n";
      } else {
        int current = pages_to_users[user_to_pages[id]];
        cout << 1.0 * (total_users - current) / (total_users - 1) << "\n";
      }
    } else {
      throw logic_error("Unknown operation");
    }
  }
}