#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

void CheckIfSliceFormsPalindromAndMirrored(const std::vector<std::string> &input, size_t index,
                                           const std::unordered_map<std::string, size_t> &input_compact_form,
                                           size_t &pairs_quantity,
                                           std::vector<std::pair<size_t, size_t>> &indices_pairs,
                                           bool left_or_right) {
    size_t input_size = input[index].size();
    for (size_t cut_pos = 0; cut_pos < input_size; ++cut_pos) {
        bool checked_part_is_palindrom = false;
        std::string complement = left_or_right ? input[index].substr(0, cut_pos) : input[index].substr(cut_pos);
        if ((cut_pos > 0) || (left_or_right)) {
            size_t left = left_or_right ? cut_pos : 0;
            size_t right = left_or_right ? input_size - 1 : cut_pos - 1;
            while ((left < right) && (input[index][left] == input[index][right])) {
                ++left;
                --right;
            }

            if (left >= right) {
                checked_part_is_palindrom = true;
            }
        }

        if (((left_or_right) && (checked_part_is_palindrom)) ||
            ((!left_or_right) && ((cut_pos == 0) || (checked_part_is_palindrom)))) {
            auto if_complement_exists = input_compact_form.find(complement);
            if ((if_complement_exists != input_compact_form.end()) && (index != if_complement_exists->second)) {
                ++pairs_quantity;

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
    std::unordered_map<std::string, size_t> input_compact_form{};
    size_t pairs_quantity = 0;
    std::vector<std::pair<size_t, size_t>> indices_pairs{};
    for (size_t i = 0; i < n; ++i) {
        std::cin >> input[i];
        input_compact_form[std::string(input[i].rbegin(), input[i].rend())] = i;
    }

    for (size_t i = 0; i < n; ++i) {
        CheckIfSliceFormsPalindromAndMirrored(input, i, input_compact_form, pairs_quantity, indices_pairs, false);
        CheckIfSliceFormsPalindromAndMirrored(input, i, input_compact_form, pairs_quantity, indices_pairs, true);
    }

    std::cout << pairs_quantity << std::endl;
    for (size_t i = 0; i < indices_pairs.size(); ++i) {
        std::cout << indices_pairs[i].first + 1 << " " << indices_pairs[i].second + 1 << std::endl;
    }

    return 0;
}