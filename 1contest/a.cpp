#include <cassert>
#include <iostream>
#include <vector>

namespace consts
{
    const char kSeparator = '#';
}

std::vector<size_t> KmpCalculatePrefixes(std::string &str) {
    size_t str_size = str.size();
    std::vector<size_t> prefixes(str_size, 0);
    prefixes[0] = 0;
    for (size_t i = 1; i < str_size; ++i) {
        int possible_border_length = prefixes[i - 1];
        while ((possible_border_length > 0) && (str[i] != str[possible_border_length])) {
            possible_border_length = prefixes[possible_border_length - 1];
        }
        prefixes[i] = str[i] == str[possible_border_length] ? possible_border_length + 1 : possible_border_length;
    }

    return prefixes;
}

std::vector<size_t> Kmp(std::string &text, std::string &pattern) {
    size_t text_size = text.size();
    size_t pattern_size = pattern.size();
    std::string kmp_string = pattern + consts::kSeparator + text;

    std::vector<size_t> beginnings{};
    std::vector<size_t> prefixes = KmpCalculatePrefixes(kmp_string);

    for (size_t i = 0; i < text_size; ++i) {
        if (prefixes[i + pattern_size + 1] == pattern_size) {
            beginnings.push_back(i + 1 - pattern_size);
        }
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
