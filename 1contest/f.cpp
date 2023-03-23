#include <cassert>
#include <iostream>
#include <vector>

const int kAlphabetSize = 26;

std::vector<int> PrefixFromZFunc(std::vector<int> &z_func) {
    int z_func_size = static_cast<int>(z_func.size());
    std::vector<int> prefixes(z_func_size, 0);

    for (int i = 1; i < z_func_size; ++i) {
        for (int j = z_func[i]; j > 0; --j) {
            if (prefixes[i + j - 1] > 0) {
                break;
            }

            prefixes[i + j - 1] = j;
        }
    }

    return prefixes;
}

std::vector<char> MinString(std::vector<int> &prefixes) {
    int str_size = static_cast<int>(prefixes.size());
    std::vector<char> answer(str_size, 0);

    for (int i = 0; i < str_size; ++i) {
        if (prefixes[i] == 0) {
            std::vector<bool> alphabet(kAlphabetSize, false);
            int index = i;
            while (index > 0) {
                index = prefixes[index - 1];
                alphabet[answer[index] - 'a'] = true;
            }
            int alphabet_index = 0;
            while (alphabet[alphabet_index]) {
                ++alphabet_index;
            }
            switch (alphabet_index)  // howdy, clang-tidy?
            {
                case 0: {
                    answer[i] = 'a';

                    break;
                }
                case 1: {
                    answer[i] = 'b';

                    break;
                }
                case 2: {
                    answer[i] = 'c';

                    break;
                }
                case 3: {
                    answer[i] = 'd';

                    break;
                }
                case 4: {
                    answer[i] = 'e';

                    break;
                }
                case 5: {
                    answer[i] = 'f';

                    break;
                }
                case 6: {
                    answer[i] = 'g';

                    break;
                }
                case 7: {
                    answer[i] = 'h';

                    break;
                }
                case 8: {
                    answer[i] = 'i';

                    break;
                }
                case 9: {
                    answer[i] = 'j';

                    break;
                }
                case 10: {
                    answer[i] = 'k';

                    break;
                }
                case 11: {
                    answer[i] = 'l';

                    break;
                }
                case 12: {
                    answer[i] = 'm';

                    break;
                }
                case 13: {
                    answer[i] = 'n';

                    break;
                }
                case 14: {
                    answer[i] = 'o';

                    break;
                }
                case 15: {
                    answer[i] = 'p';

                    break;
                }
                case 16: {
                    answer[i] = 'q';

                    break;
                }
                case 17: {
                    answer[i] = 'r';

                    break;
                }
                case 18: {
                    answer[i] = 's';

                    break;
                }
                case 19: {
                    answer[i] = 't';

                    break;
                }
                case 20: {
                    answer[i] = 'u';

                    break;
                }
                case 21: {
                    answer[i] = 'v';

                    break;
                }
                case 22: {
                    answer[i] = 'w';

                    break;
                }
                case 23: {
                    answer[i] = 'x';

                    break;
                }
                case 24: {
                    answer[i] = 'y';

                    break;
                }
                case 25: {
                    answer[i] = 'z';

                    break;
                }
            }

        } else {
            answer[i] = answer[prefixes[i] - 1];
        }
    }

    return answer;
}

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<int> z_func(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> z_func[i];
    }
    std::vector<int> prefixes = PrefixFromZFunc(z_func);
    std::vector<char> result = MinString(prefixes);
    for (size_t i = 0; i < result.size(); ++i) {
        std::cout << result[i];
    }

    return 0;
}