#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

/// \brief For string with certain index in input decides if there is matching among others ones that being concatenated they form a palindrom
/// \param input - reference to std::vector<std::string>, pool of inputted strings
/// \param index - index of investigated string
/// \param input_compact_form - reference to std::unordered map containing all strings from input being reversed
/// \param indices_pairs - non-const reference of pairs of indices of strings that being concatenateed form a palindrom
/// \param left_or_right - determines whether we want to find a string that is similar to the left part of investigated string or to the right one;
///                        false means left, true - right
void CheckIfSliceFormsPalindromAndMirrored(const std::vector<std::string> &input, size_t index,
                                           const std::unordered_map<std::string, size_t> &input_compact_form,
                                           std::vector<std::pair<size_t, size_t>> &indices_pairs, bool left_or_right) {
    auto input_iterator = input.cbegin() + static_cast<std::vector<std::string>::difference_type>(index);
    size_t input_size = input_iterator->size();
    for (size_t cut_pos = 0; cut_pos < input_size; ++cut_pos) {
        bool checked_part_is_palindrom = false;
        std::string complement = left_or_right ? input_iterator->substr(0, cut_pos) : input_iterator->substr(cut_pos);
        if ((cut_pos > 0) || (left_or_right)) {
            auto cur_string_begin = input_iterator->cbegin();
            auto cur_string_left_iterator = cur_string_begin;
            if (left_or_right) {
                cur_string_left_iterator += static_cast<std::string::difference_type>(cut_pos);
            }
            auto cur_string_right_iterator =
                left_or_right ? input_iterator->end() - 1
                              : cur_string_begin + static_cast<std::string::difference_type>(cut_pos - 1);
            while ((cur_string_left_iterator < cur_string_right_iterator) &&
                   (*cur_string_left_iterator == *cur_string_right_iterator)) {
                ++cur_string_left_iterator;
                --cur_string_right_iterator;
            }

            if (cur_string_left_iterator >= cur_string_right_iterator) {
                checked_part_is_palindrom = true;
            }
        }

        if (((left_or_right) && (checked_part_is_palindrom)) ||
            ((!left_or_right) && ((cut_pos == 0) || (checked_part_is_palindrom)))) {
            auto if_complement_exists = input_compact_form.find(complement);
            if ((if_complement_exists != input_compact_form.end()) && (index != if_complement_exists->second)) {

                if (left_or_right) {
                    indices_pairs.emplace_back(index, if_complement_exists->second);
                } else {
                    indices_pairs.emplace_back(if_complement_exists->second, index);
                }
            }
        }
    }
}

int main() {
    size_t n = 0;
    std::cin >> n;

    std::vector<std::string> input(n);
    auto input_begin = input.begin();
    auto input_iterator = input_begin;
    auto input_end = input.end();
    std::unordered_map<std::string, size_t> input_compact_form{};
    while (input_iterator != input_end) {
        std::cin >> *input_iterator;
        input_compact_form[std::string(input_iterator->crbegin(), input_iterator->crend())] =
            input_iterator - input_begin;

        ++input_iterator;
    }

    std::vector<std::pair<size_t, size_t>> indices_pairs{};
    for (size_t i = 0; i < n; ++i) {
        CheckIfSliceFormsPalindromAndMirrored(input, i, input_compact_form, indices_pairs, false);
        CheckIfSliceFormsPalindromAndMirrored(input, i, input_compact_form, indices_pairs, true);
    }

    std::cout << indices_pairs.size() << std::endl;
    auto indices_pairs_iterator = indices_pairs.cbegin();
    auto indices_pairs_end = indices_pairs.cend();
    while (indices_pairs_iterator != indices_pairs_end) {
        std::cout << indices_pairs_iterator->first + 1 << " " << indices_pairs_iterator->second + 1 << std::endl;

        ++indices_pairs_iterator;
    }

    return 0;
}