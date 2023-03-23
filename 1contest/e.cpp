#include <cassert>
#include <iostream>
#include <vector>

std::vector<int> PositionsInFrontOfMirror(const std::vector<int> &colors) {
    int colors_size = static_cast<int>(colors.size());
    std::vector<int> palindromes_in_position(colors_size, 0);

    int big_palindrom_left_index = 0;
    int big_palindrom_right_index = -1;

    for (int i = 0; i < colors_size; ++i) {
        int cur_palindrom_size = 0;
        if (i <= big_palindrom_right_index) {
            cur_palindrom_size =
                std::min(big_palindrom_right_index - i + 1,
                         palindromes_in_position[big_palindrom_right_index - i + big_palindrom_left_index + 1]);
        }

        while ((i + cur_palindrom_size < colors_size) && (i - cur_palindrom_size > 0) &&
               (colors[i - cur_palindrom_size - 1] == colors[i + cur_palindrom_size])) {
            ++cur_palindrom_size;
        }

        palindromes_in_position[i] = cur_palindrom_size;

        if (i + cur_palindrom_size - 1 > big_palindrom_right_index) {
            big_palindrom_left_index = i - cur_palindrom_size;
            big_palindrom_right_index = i + cur_palindrom_size - 1;
        }
    }

    return palindromes_in_position;
}

int main() {
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    std::vector<int> colors(n, 0);
    for (int i = 0; i < n; ++i) {
        std::cin >> colors[i];
    }

    std::vector<int> result = PositionsInFrontOfMirror(colors);
    for (size_t i = result.size(); i > 0; --i) {
        if (result[i - 1] == static_cast<int>(i - 1)) {
            std::cout << n - i + 1 << " ";
        }
    }

    return 0;
}
