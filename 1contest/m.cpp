#include <algorithm>
#include <iostream>
#include <vector>

/*! \class SuffixContext
    \brief Information about suffixes of given string

    initial_index - index where suffix starts in string
    rank - helper index indicating the order in which first parts of suffixes are sorted
    next_rank - helper index indicating the order in which second parts of suffixes are sorted
*/
struct SuffixContext {
    int initial_index = 0;
    int rank = 0;
    int next_rank = 0;
};

/// \brief Compares two suffixes according to their rank and next rank
/// \param suf1 - reference to first  SuffixContext
/// \param suf2 - reference to second SuffixContext
/// \return true if suf1 strictly lexicographically smaller than suf2; false otherwise
bool Comparator(const SuffixContext &suf1, const SuffixContext &suf2) {
    return suf1.rank < suf2.rank ? true : suf1.rank == suf2.rank ? suf1.next_rank < suf2.next_rank : false;
}

/// \brief Builds suffix array of given string
/// \param str - reference to given string
/// \return std::vector<int> - suffix array where each suffix is represented by index where it starts in str (beginning with 1)
std::vector<int> BuildSufArr(const std::string &str) {
    int str_size = static_cast<int>(str.size());
    std::vector<SuffixContext> suffixes(str_size);
    auto suffixes_begin = suffixes.begin();
    auto suffixes_end = suffixes.cend();
    auto suffixes_iterator = suffixes_begin;
    while (suffixes_iterator != suffixes_end) {
        int suffix_index = static_cast<int>(suffixes_iterator - suffixes_begin);
        suffixes_iterator->initial_index = suffix_index;
        auto str_iterator = str.cbegin() + static_cast<std::string::difference_type>(suffix_index);
        suffixes_iterator->rank = *str_iterator;
        suffixes_iterator->next_rank = suffix_index == str_size - 1 ? -1 : *(str_iterator + 1);

        ++suffixes_iterator;
    }

    std::sort(suffixes.begin(), suffixes.end(), Comparator);

    for (int sorted_length = 2; sorted_length < str_size; sorted_length *= 2) {
        int prev_rank = suffixes_begin->rank;
        suffixes_begin->rank = 0;

        std::vector<int> intermediary_order(str_size);
        auto intermediary_order_begin = intermediary_order.begin();
        auto intermediary_order_iterator =
            intermediary_order_begin + static_cast<std::vector<int>::difference_type>(suffixes_begin->initial_index);
        *intermediary_order_iterator = 0;
        int cur_rank = 0;
        suffixes_iterator = suffixes_begin + 1;
        while (suffixes_iterator != suffixes_end) {
            bool equal_to_prev = false;

            if ((suffixes_iterator->rank == prev_rank) &&
                (suffixes_iterator->next_rank == (suffixes_iterator - 1)->next_rank)) {
                equal_to_prev = true;
            }

            prev_rank = suffixes_iterator->rank;
            if (!equal_to_prev) {
                ++cur_rank;
            }
            suffixes_iterator->rank = cur_rank;

            *(intermediary_order_begin +
              static_cast<std::vector<int>::difference_type>(suffixes_iterator->initial_index)) =
                static_cast<int>(suffixes_iterator - suffixes_begin);

            ++suffixes_iterator;
        }

        suffixes_iterator = suffixes_begin;
        while (suffixes_iterator != suffixes_end) {
            int next_suffix_real_index = suffixes_iterator->initial_index + sorted_length;
            suffixes_iterator->next_rank =
                next_suffix_real_index >= str_size
                    ? -1
                    : (suffixes_begin + static_cast<std::vector<SuffixContext>::difference_type>(
                                            *(intermediary_order_begin +
                                              static_cast<std::vector<int>::difference_type>(next_suffix_real_index))))
                          ->rank;

            ++suffixes_iterator;
        }

        std::sort(suffixes.begin(), suffixes.end(), Comparator);
    }

    std::vector<int> result(str_size);
    auto result_end = result.cend();
    auto result_iterator = result.begin();
    suffixes_iterator = suffixes_begin;
    while (result_iterator != result_end) {
        *result_iterator = suffixes_iterator->initial_index + 1;

        ++result_iterator;
        ++suffixes_iterator;
    }

    return result;
}

int main() {
    std::string input;
    getline(std::cin, input);
    std::vector<int> result = BuildSufArr(input);
    auto result_iterator = result.cbegin();
    auto result_end = result.cend();
    while (result_iterator != result_end) {
        std::cout << *result_iterator << " ";

        ++result_iterator;
    }

    return 0;
}
