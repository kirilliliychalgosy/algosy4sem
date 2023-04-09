#include <cassert>
#include <iostream>
#include <vector>

namespace consts
{
    const char kSeparator = '#';
}

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

std::vector<bool> GetInterestingness(std::vector<size_t> &prefixes) {
    size_t prefixes_size = prefixes.size();
    std::vector<bool> sections(prefixes_size + 1, false);  // needa last element for yes/no
    if (prefixes[0] == 0) {
        sections[prefixes_size] = true;
    } else {
        sections[0] = true;
        for (size_t i = prefixes_size - 1; i > 0; --i) {
            if (prefixes[i] == 0) {
                sections[prefixes_size] = true;

                break;
            }

            if (prefixes[i - 1] >= prefixes[i]) {
                sections[i + 1 - prefixes[i]] = true;
            }
        }
    }

    return sections;
}

template <typename T>
std::vector<size_t> GetSubvector(std::vector<T> &v, size_t start, size_t size) {
    std::vector<T> subvector(size);
    for (size_t i = 0; i < size; ++i) {
        subvector[i] = v[start + i];
    }

    return subvector;
}

int main() {
    std::string fiona_word;
    std::string donkey_word;
    std::cin >> fiona_word >> donkey_word;

    std::string concatenated = fiona_word + consts::kSeparator + donkey_word;
    size_t fiona_word_size = fiona_word.size();
    size_t donkey_word_size = donkey_word.size();
    std::vector<size_t> prefixes = CalculatePrefixes(concatenated);
    size_t donkey_word_in_concatenated_start_index = fiona_word_size + 1;

    std::vector<size_t> prefixes_for_donkey_word =
        GetSubvector(prefixes, donkey_word_in_concatenated_start_index, donkey_word_size);
    std::vector<bool> result = GetInterestingness(prefixes_for_donkey_word);
    if (result[donkey_word_size])  // remember we have additional indicator element at the end
    {
        std::cout << "Yes";
    } else {
        std::cout << "No" << std::endl;

        size_t word_start = 0;
        while (word_start < donkey_word_size) {
            size_t str_iter = word_start;
            while ((str_iter < donkey_word_size) && (!result[str_iter] || (str_iter == word_start))) {
                std::cout << donkey_word[str_iter];

                ++str_iter;
            }
            std::cout << " ";

            word_start = str_iter;
        }
    }

    return 0;
}
