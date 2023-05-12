#include <cassert>
#include <iostream>
#include <iterator>
#include <vector>

namespace string_separators {
const char kSeparator = '#';
}

/// \brief Calculates value of prefix function
/// \param str - reference to string prefix function is being calculated of
/// \return std::vector<size_t> with prefix function values
std::vector<size_t> CalculatePrefixes(const std::string &str) {
    size_t str_size = str.size();
    std::vector<size_t> prefixes(str_size, 0);
    auto prefixes_begin = prefixes.begin();
    auto prefixes_iterator = prefixes_begin + 1;
    *prefixes_begin = 0;
    auto str_begin = str.begin();
    auto str_end = str.end();
    auto str_iterator = str_begin + 1;
    while (str_iterator != str_end) {
        size_t possible_border_length = *(prefixes_iterator - 1);
        while ((possible_border_length > 0) &&
               (*str_iterator !=
                *(str_begin + static_cast<std::vector<size_t>::difference_type>(possible_border_length)))) {
            possible_border_length =
                *(prefixes_begin + static_cast<std::vector<size_t>::difference_type>(possible_border_length) - 1);
        }
        *prefixes_iterator = possible_border_length;
        if (*str_iterator == *(str_begin + static_cast<std::vector<size_t>::difference_type>(possible_border_length))) {
            ++(*prefixes_iterator);
        }

        ++str_iterator;
        ++prefixes_iterator;
    }

    return prefixes;
}

/// \brief Knuth-Morris-Pratt algorithm
/// \param text - reference to string where pattern is being searched
/// \param pattern - reference to string that is being searched in text
/// \return std::vector<size_t> with indexes of pattern enterings in text
std::vector<size_t> Kmp(const std::string &text, const std::string &pattern) {
    std::string kmp_string = pattern + string_separators::kSeparator + text;
    std::vector<size_t> prefixes = CalculatePrefixes(kmp_string);

    size_t pattern_size = pattern.size();
    auto prefixes_begin = prefixes.begin();
    auto prefixes_end = prefixes.end();
    auto prefixes_iterator = prefixes_begin + static_cast<std::vector<size_t>::difference_type>(pattern_size) + 1;
    std::vector<size_t> beginnings{};
    while (prefixes_iterator != prefixes_end) {
        if (*prefixes_iterator == pattern_size) {
            beginnings.push_back(prefixes_iterator - prefixes_begin - 2 * pattern_size);
        }

        ++prefixes_iterator;
    }

    return beginnings;
}

int main() {
    std::string text;
    std::string pattern;
    std::cin >> text >> pattern;

    std::vector<size_t> answer = Kmp(text, pattern);
    for (size_t i = 0; i < answer.size(); i++) {
        std::cout << answer[i] << std::endl;
    }

    return 0;
}
