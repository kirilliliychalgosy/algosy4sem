#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

const char kSeparator = '#';

std::vector<size_t> CalculatePrefixes(const std::string &str) {
    size_t str_size = str.size();
    std::vector<size_t> prefixes(str_size, 0);
    for (size_t i = 1; i < str_size; ++i) {
        size_t possible_border_length = prefixes[i - 1];
        while ((possible_border_length > 0) && (str[i] != str[possible_border_length])) {
            possible_border_length = prefixes[possible_border_length - 1];
        }
        prefixes[i] = str[i] == str[possible_border_length] ? possible_border_length + 1 : possible_border_length;
    }

    return prefixes;
}

std::vector<size_t> CalculateZFunc(const std::string &str) {
    size_t str_size = str.size();
    std::vector<size_t> z_func(str_size, 0);
    size_t left = 0;
    size_t right = 0;
    for (size_t i = 1; i < str_size; ++i) {
        z_func[i] = i < right ? std::min(z_func[i - left], right - i) : 0;
        while ((i + z_func[i] < str_size) && (str[z_func[i]] == str[i + z_func[i]])) {
            ++z_func[i];
        }

        if (i + z_func[i] > right) {
            left = i;
            right = i + z_func[i];
        }
    }

    return z_func;
}

size_t CountEntries(std::string &pattern, std::string &text) {
    size_t pattern_size = pattern.size();

    std::string pattern_text_concatenated = pattern + kSeparator + text;
    std::reverse(pattern.begin(), pattern.end());
    std::reverse(text.begin(), text.end());
    std::string reversed_pattern_text_concatenated = pattern + kSeparator + text;

    std::vector<size_t> prefixes = CalculatePrefixes(pattern_text_concatenated);
    std::vector<size_t> z_func_on_reversed = CalculateZFunc(reversed_pattern_text_concatenated);
    size_t answer = 0;
    size_t concatenated_str_size = pattern_text_concatenated.size();
    for (size_t i = pattern_size + 1; i < concatenated_str_size; ++i) {
        if ((prefixes[i] >= pattern_size) ||
            ((i - prefixes[i] >= pattern_size + 1) &&
             (z_func_on_reversed[pattern_size + 1 + (concatenated_str_size - 1 - (i - prefixes[i]))] + prefixes[i] >=
              pattern_size))) {
            ++answer;
        }
    }

    return answer;
}

int main() {
    std::string pattern;
    std::string text;
    std::cin >> pattern >> text;

    std::cout << CountEntries(pattern, text);

    return 0;
}
