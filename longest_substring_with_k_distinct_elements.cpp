#include <bits/stdc++.h>
#include <string.h>
using namespace std;

int find_substring_with_K_distinct_characters(string a, int k)
{
    int begin = 0, end = 0;
    int high = 0, low = 0;
    cout << "hai" << a.size();
    unordered_set<char> distinct_character;
    unordered_map<char, int> char_count;
    if (a.size() > 1)
    {
        distinct_character.insert(a[end]);
    }

    while (end < a.size() - 1)
    {
        end++;
        distinct_character.insert(a[end]);
        char_count[a[end]]++;
        while (distinct_character.size() > k)
        {
            char_count[a[begin]]--;
            if (char_count[a[begin]] == 0)
            {
                distinct_character.erase(a[begin]);
            }
            begin++;
            cout << "\nNow low is " << begin << " distinct character size " << distinct_character.size() << "\n";
        }

        cout << "begin" << begin << "end:" << end << "\n";
        if (end - begin > high - low)
        {
            low = begin;
            high = end;
        }
    }
    return high - low + 1;
}

int main()
{
    string s = "ababadefg";
    int k = find_substring_with_K_distinct_characters(s, 2);
    cout << "\nThe result is" << k << "\n";
    return 0;
}
