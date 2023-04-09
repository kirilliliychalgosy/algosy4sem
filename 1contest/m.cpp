#include <algorithm>
#include <iostream>
#include <vector>

struct SuffixContext {
    int initial_index = 0;
    int rank = 0;
    int next_rank = 0;
};

bool Comparator(const SuffixContext &suf1, const SuffixContext &suf2) {
    return suf1.rank < suf2.rank ? true : suf1.rank == suf2.rank ? suf1.next_rank < suf2.next_rank : false;
}

std::vector<int> BuildSufArr(const std::string &str) {
    int str_size = static_cast<int>(str.size());
    std::vector<SuffixContext> suffixes(str_size);

    for (int suffix_index = 0; suffix_index < str_size; ++suffix_index) {
        suffixes[suffix_index].initial_index = suffix_index;
        suffixes[suffix_index].rank = str[suffix_index];
        suffixes[suffix_index].next_rank = suffix_index == str_size - 1 ? -1 : str[(suffix_index + 1)];
    }

    std::sort(suffixes.begin(), suffixes.end(), Comparator);

    for (int sorted_length = 2; sorted_length < str_size; sorted_length *= 2) {
        int prev_rank = suffixes[0].rank;
        suffixes[0].rank = 0;

        std::vector<int> intermediary_order(str_size);
        intermediary_order[suffixes[0].initial_index] = 0;
        int cur_rank = 0;
        for (int suffix_index = 1; suffix_index < str_size; ++suffix_index) {
            bool equal_to_prev = false;

            if ((suffixes[suffix_index].rank == prev_rank) &&
                (suffixes[suffix_index].next_rank == suffixes[suffix_index - 1].next_rank)) {
                equal_to_prev = true;
            }

            prev_rank = suffixes[suffix_index].rank;
            if (!equal_to_prev) {
                ++cur_rank;
            }
            suffixes[suffix_index].rank = cur_rank;

            intermediary_order[suffixes[suffix_index].initial_index] = suffix_index;
        }

        for (int suffix_index = 0; suffix_index < str_size; ++suffix_index) {
            int next_suffix_real_index = suffixes[suffix_index].initial_index + sorted_length;
            suffixes[suffix_index].next_rank =
                next_suffix_real_index >= str_size ? -1 : suffixes[intermediary_order[next_suffix_real_index]].rank;
        }

        std::sort(suffixes.begin(), suffixes.end(), Comparator);
    }

    std::vector<int> result(str_size);
    for (int i = 0; i < str_size; ++i) {
        result[i] = suffixes[i].initial_index + 1;
    }

    return result;
}

int main() {
    std::string input;
    getline(std::cin, input);
    std::vector<int> result = BuildSufArr(input);
    int result_size = static_cast<int>(result.size());
    for (int i = 0; i < result_size; ++i) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}
