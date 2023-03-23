#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

const char kSeparator = '#';

std::vector<size_t> CalculatePrefixes(std::string &str) {
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

std::string Concatenation228(std::vector<std::string> &words) {
    std::string answer = words[0];
    for (size_t i = 1; i < words.size(); ++i) {
        size_t answer_size = answer.size();
        size_t next_string_size = words[i].size();
        size_t cur_string_taken_suffix_start = answer_size > next_string_size ? answer_size - next_string_size : 0;
        size_t cur_string_taken_suffix_size = answer_size > next_string_size ? next_string_size : answer_size;

        std::string string_for_overlay_calculation =
            words[i] + kSeparator + answer.substr(cur_string_taken_suffix_start, cur_string_taken_suffix_size);
        size_t string_for_overlay_calculation_size = string_for_overlay_calculation.size();
        std::vector<size_t> prefixes = CalculatePrefixes(string_for_overlay_calculation);

        size_t overlay_size = prefixes[string_for_overlay_calculation_size - 1];
        answer += words[i].substr(overlay_size, next_string_size - overlay_size);
    }

    return answer;
}

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<std::string> input(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> input[i];
    }

    std::cout << Concatenation228(input);

    return 0;
}