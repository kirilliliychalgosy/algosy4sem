#include <cassert>
#include <iostream>
#include <map>
#include <vector>

struct SuffAutomation {
    struct State {
        int max_len_ = 0;
        int suf_link_ = 0;
        std::map<char, int> to_;
    };

    SuffAutomation() : last_state_index(0), size_(1) {
        states_.push_back({0, -1, std::map<char, int>()});
    }

    void AddSymb(char symb) {
        ++str_size;  // required for current task

        int new_state_index = size_;
        ++size_;

        states_.push_back({states_[last_state_index].max_len_ + 1, 0, std::map<char, int>()});

        int cur_state_index = last_state_index;
        while ((cur_state_index != -1) && (states_[cur_state_index].to_.count(symb) == 0)) {
            states_[cur_state_index].to_[symb] = new_state_index;
            cur_state_index = states_[cur_state_index].suf_link_;
        }
        if (cur_state_index == -1) {
            states_[new_state_index].suf_link_ = 0;
        } else {
            int state_travelled_to_by_symb_index = states_[cur_state_index].to_[symb];
            if (states_[cur_state_index].max_len_ + 1 == states_[state_travelled_to_by_symb_index].max_len_) {
                states_[new_state_index].suf_link_ = state_travelled_to_by_symb_index;
            } else {
                int clone_index = size_;
                ++size_;

                states_.push_back({states_[cur_state_index].max_len_ + 1,
                                   states_[state_travelled_to_by_symb_index].suf_link_,
                                   states_[state_travelled_to_by_symb_index].to_});

                while ((cur_state_index != -1) &&
                       (states_[cur_state_index].to_[symb] == state_travelled_to_by_symb_index)) {
                    states_[cur_state_index].to_[symb] = clone_index;
                    cur_state_index = states_[cur_state_index].suf_link_;
                }

                states_[state_travelled_to_by_symb_index].suf_link_ = clone_index;
                states_[new_state_index].suf_link_ = clone_index;
            }
        }

        last_state_index = new_state_index;
    }

    int last_state_index = 0;
    int size_ = 0;
    std::vector<State> states_;

    //--------------required for current task---------------------

    size_t last_string_different_substrings_quantity = 0;
    int str_size = 0;

    size_t GetCurStringDifferentSubstringsQuantity()  // should be called after each symbol added!
    {
        last_string_different_substrings_quantity += static_cast<size_t> (str_size - states_[states_[last_state_index].suf_link_].max_len_);

        return last_string_different_substrings_quantity;
    }
};

int main() {
    std::string input;
    std::cin >> input;
    int input_size = static_cast<int>(input.size());
    SuffAutomation ac_val;
    for (int i = 0; i < input_size; ++i) {
        ac_val.AddSymb(input[i]);
        size_t result = ac_val.GetCurStringDifferentSubstringsQuantity();
        std::cout << result << std::endl;
    }

    return 0;
}
