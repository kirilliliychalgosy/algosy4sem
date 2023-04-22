#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <optional>
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
    auto str_begin = str.cbegin();
    auto str_end = str.cend();
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


/// \brief Finds how word can be separated into prefixes of text
/// \param prefixes - reference to part(!) of prefix function of string text#word, where # is a separator
/// \return If separation is possible, std::vector<size_t> with indexes of prefixes of text to which word is separated;
///         otherwise, std::nullopt is returned
std::optional<std::vector<size_t>> FindSeparationOfWordIntoTextPrefixes(const std::vector<size_t> &prefixes) {
    std::vector<size_t> prefixes_beginnings;
    if (*prefixes.cbegin() == 0) {
        return std::nullopt;
    }
    auto prefixes_iterator = prefixes.cend() - 1;
    auto prefixes_begin = prefixes.cbegin();
    while (prefixes_iterator != prefixes_begin) {
        if (*prefixes_iterator == 0) {
            return std::nullopt;
        }

        if (*(prefixes_iterator - 1) >= *prefixes_iterator) {
            prefixes_beginnings.push_back(prefixes_iterator - prefixes_begin + 1 - *prefixes_iterator);
        }

        --prefixes_iterator;
    }

    prefixes_beginnings.push_back(0);

    std::reverse(prefixes_beginnings.begin(), prefixes_beginnings.end());

    return prefixes_beginnings;
}

/// \brief Prints prefixes fo text to which word is separated separately
/// \param word - word which is separated
/// \param prefixes_beginnings - vector which contains indexes of prefixes of text to which word is separated
void PrintSeparationOfWordIntoTextPrefixes(const std::string &word, const std::vector<size_t> &prefixes_beginnings) {
    size_t prefixes_beginnings_size = prefixes_beginnings.size();
    if (prefixes_beginnings_size == 1) {
        std::cout << word;
    } else {
        auto prefixes_beginnings_iterator = prefixes_beginnings.cbegin() + 1;
        auto prefixes_beginnings_end = prefixes_beginnings.cend();
        while (prefixes_beginnings_iterator != prefixes_beginnings_end) {
            size_t delta = *prefixes_beginnings_iterator - *(prefixes_beginnings_iterator - 1);

            std::cout << word.substr(0, delta) << " ";

            ++prefixes_beginnings_iterator;
        }

        std::cout << word.substr(*(prefixes_beginnings_end - 1), word.size());
    }
}

int main() {
    std::string text;
    std::string might_consist_of_text_prefixes_word;
    std::cin >> text >> might_consist_of_text_prefixes_word;

    std::string concatenated = text + string_separators::kSeparator + might_consist_of_text_prefixes_word;
    size_t text_size = text.size();
    std::vector<size_t> prefixes = CalculatePrefixes(concatenated);
    size_t might_consist_of_text_prefixes_word_in_concatenated_start_index = text_size + 1;

    std::vector<size_t> prefixes_for_might_consist_of_text_prefixes_word(
        prefixes.cbegin() + static_cast<std::vector<size_t>::difference_type>(
                                might_consist_of_text_prefixes_word_in_concatenated_start_index),
        prefixes.cend());
    std::optional<std::vector<size_t>> result =
        FindSeparationOfWordIntoTextPrefixes(prefixes_for_might_consist_of_text_prefixes_word);
    if (result == std::nullopt) {
        std::cout << "Yes";
    } else {
        std::cout << "No" << std::endl;

        std::sort(result.value().begin(), result.value().end());

        PrintSeparationOfWordIntoTextPrefixes(might_consist_of_text_prefixes_word, result.value());
    }

    return 0;
}
