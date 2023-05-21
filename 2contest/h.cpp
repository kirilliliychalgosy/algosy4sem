#include <cassert>
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

#define _USE_MATH_DEFINES  // for number pi

struct FFTButterflyTransformationInPlaceContext {
    FFTButterflyTransformationInPlaceContext()
        : fft_elem_with_plus_ptr(nullptr)
        , fft_elem_with_minus_ptr(nullptr)
        , fft_elem_update_until_ptr(nullptr)
        , fft_root_from_1_ptr(nullptr) {
    }

    void ToNextData() {
        ++fft_elem_with_plus_ptr;
        ++fft_elem_with_minus_ptr;
        ++fft_root_from_1_ptr;
    }

    //-----------------------------------Variables-------------------------------------
    std::complex<double> *fft_elem_with_plus_ptr;
    std::complex<double> *fft_elem_with_minus_ptr;
    std::complex<double> *fft_elem_update_until_ptr;
    std::complex<double> *fft_root_from_1_ptr;
};

/// \brief FFT special: reorders coefficients of polynomial in certain way
/// \param size - quantity of coefficients of polynomial
/// \param size_log - binary logarithm of size
/// \return std::vector<int64_t> - order of indexes symbolizing order of coefficients
std::vector<int64_t> GetBitsReverse(int64_t size, int64_t size_log) {
    std::vector<int64_t> result(size);

    for (int64_t i = 0; i < size; ++i) {
        result[i] = 0;
        for (int64_t j = 0; j < size_log; ++j) {
            if (i & (1 << j)) {
                result[i] |= (1 << (size_log - 1 - j));
            }
        }
    }

    return result;
}

/// \brief Counts binary logarithm (rounds towards zero)
/// \param value - value binary logarithm is being counted of
/// \return Binary logarithm of value
int64_t GetLog2(int64_t value) {
    int64_t result = 0;
    int64_t pow = 1;
    while (pow < value) {
        ++result;
        pow <<= 1;
    }

    return result - 1;
}

/// \brief FFT
/// \param coefs - pointer to array of polynomial coefficients (NOTE: is changed after the function call!)
/// \param size - size of coefs array
/// \param is_invert - shows whether FFT is direct or inverse
/// \param bit_reverse - reference to vector with reordering of polynomial coefficients
void DoFft(std::complex<double> *coefs, int64_t size, bool is_invert, const std::vector<int64_t> &bit_reverse) {
    for (int64_t i = 0; i < size; ++i) {
        if (i < bit_reverse[i]) {
            std::swap(coefs[i], coefs[bit_reverse[i]]);
        }
    }

    for (int64_t length = 2; length <= size; length <<= 1) {
        double angle = 2 * M_PI / static_cast<double>(length);
        if (is_invert) {
            angle *= -1;
        }

        int64_t halved_length = length >> 1;

        std::complex<double> complex_root_from_1(cos(angle), sin(angle));
        auto complex_roots_from_1 = new std::complex<double>[size];
        complex_roots_from_1[0] = {1, 0};
        for (int64_t root_index = 1; root_index < halved_length; ++root_index) {
            complex_roots_from_1[root_index] = complex_roots_from_1[root_index - 1] * complex_root_from_1;
        }

        for (int64_t i = 0; i < size; i += length)  // optimization hell
        {
            FFTButterflyTransformationInPlaceContext context;
            context.fft_elem_with_plus_ptr = coefs + i;
            context.fft_elem_with_minus_ptr = coefs + i + halved_length;
            context.fft_elem_update_until_ptr = coefs + i + halved_length;
            context.fft_root_from_1_ptr = complex_roots_from_1;

            while (context.fft_elem_with_plus_ptr < context.fft_elem_update_until_ptr) {
                std::complex<double> fft_result_part_with_root_multiplied =
                    *context.fft_elem_with_minus_ptr * *context.fft_root_from_1_ptr;
                *context.fft_elem_with_minus_ptr =
                    *context.fft_elem_with_plus_ptr - fft_result_part_with_root_multiplied;
                *context.fft_elem_with_plus_ptr += fft_result_part_with_root_multiplied;

                context.ToNextData();
            }
        }

        delete[] complex_roots_from_1;
    }

    if (is_invert) {
        for (int64_t i = 0; i < size; ++i) {
            coefs[i] /= static_cast<double>(size);
        }
    }
}

/// \brief Multiplies two polynomials
/// \param v1 - reference to vector with coefficients of the first  polynomial
/// \param v2 - reference to vector with coefficients of the second polynomial
/// \return std::vector<int64_t> - vector with coefficients of product of v1 and v2
std::vector<int64_t> PolynomialMultiplication(const std::vector<int64_t> &v1, const std::vector<int64_t> &v2) {
    uint64_t v1_size = v1.size();
    uint64_t v2_size = v2.size();
    uint64_t max_from_sizes = v1.size();
    if (max_from_sizes < v2_size) {
        max_from_sizes = v2_size;
    }
    int64_t smallest_pow_of_two_greater_than_both_vectors_sizes = 1;
    while (smallest_pow_of_two_greater_than_both_vectors_sizes < static_cast<int64_t>(max_from_sizes)) {
        smallest_pow_of_two_greater_than_both_vectors_sizes <<= 1;
    }
    smallest_pow_of_two_greater_than_both_vectors_sizes <<=
        1;  // for result of multiplication we need two times more place

    auto double_v1 = new std::complex<double>[smallest_pow_of_two_greater_than_both_vectors_sizes]();
    auto double_v2 = new std::complex<double>[smallest_pow_of_two_greater_than_both_vectors_sizes]();
    for (uint64_t i = 0; i < max_from_sizes; ++i) {
        if (i < v1_size) {
            double_v1[i] = {static_cast<double>(v1[i]), 0};
        }
        if (i < v2_size) {
            double_v2[i] = {static_cast<double>(v2[i]), 0};
        }
    }

    std::vector<int64_t> bit_reverse =
        GetBitsReverse(smallest_pow_of_two_greater_than_both_vectors_sizes,
                       static_cast<int64_t>(std::log2(smallest_pow_of_two_greater_than_both_vectors_sizes)));

    DoFft(double_v1, smallest_pow_of_two_greater_than_both_vectors_sizes, false, bit_reverse);
    DoFft(double_v2, smallest_pow_of_two_greater_than_both_vectors_sizes, false, bit_reverse);
    for (int64_t i = 0; i < smallest_pow_of_two_greater_than_both_vectors_sizes; ++i) {
        double_v1[i] *= double_v2[i];
    }
    DoFft(double_v1, smallest_pow_of_two_greater_than_both_vectors_sizes, true, bit_reverse);

    uint64_t product_actual_deg = v1_size + v2_size - 2;
    std::vector<int64_t> result(product_actual_deg + 1);
    for (uint64_t index = 0; index < product_actual_deg + 1; ++index) {
        result[index] = double_v1[product_actual_deg - index].real() >= 0
                            ?  // rounding
                            static_cast<int64_t>(double_v1[product_actual_deg - index].real() + 0.5)
                            : static_cast<int64_t>(double_v1[product_actual_deg - index].real() - 0.5);
    }

    delete[] double_v2;
    delete[] double_v1;

    return result;
}

int main() {
    int64_t n = 0;
    int64_t m = 0;

    scanf("%lu", &n);
    std::vector<int64_t> a_coefs(n + 1);
    for (int64_t i = 0; i < n + 1; ++i) {
        scanf("%ld", &a_coefs[n - i]);
    }

    scanf("%lu", &m);
    std::vector<int64_t> b_coefs(m + 1);
    for (int64_t i = 0; i < m + 1; ++i) {
        scanf("%ld", &b_coefs[m - i]);
    }

    std::vector<int64_t> mult_result = PolynomialMultiplication(a_coefs, b_coefs /*, bit_reverse*/);
    uint64_t result_size = mult_result.size();
    printf("%lu ", result_size - 1);
    for (uint64_t i = 0; i < result_size; ++i) {
        printf("%ld ", mult_result[i]);
    }

    return 0;
}
