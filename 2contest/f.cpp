#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

/// Trait (*): if sqrt(+) is such integer that sqrt * sqrt <= value, but (sqrt + 1) * (sqrt + 1) > value,
///            then both number <= sqrt and value / number are matching it, number is natural

/// \brief Counts how much numbers match (*)
/// \param value - for which quantity of numbers that match (*) is being calculated
/// \return quantity of such numbers
uint64_t GetSubsqrtsAndPairedQuantity(uint64_t value) {
    assert(value != 0);

    if (value == 1) {
        return 1;
    }

    uint64_t result = 2;
    uint64_t subsqrt = 2;
    for (subsqrt = 2; subsqrt * subsqrt < value; ++subsqrt) {
        result += 2;
    }
    if ((subsqrt - 1) * subsqrt > value) {
        --result;
    } else if (subsqrt * subsqrt == value) {
        ++result;
    }

    return result;
}

/// \brief Forms vector with numbers that match (*)
/// \param value - for which numbers that match (*) are put in the vector
/// \return std::vector<uint64_t> with numbers that match (*)
std::vector<uint64_t> GetSubsqrtsAndPaired(uint64_t value) {
    assert(value != 0);

    uint64_t subsqrts_and_paired_quantity = GetSubsqrtsAndPairedQuantity(value);
    std::vector<uint64_t> subsqrts_and_paired(subsqrts_and_paired_quantity);
    subsqrts_and_paired[0] = 1;

    if (subsqrts_and_paired_quantity != 1) {
        subsqrts_and_paired[subsqrts_and_paired_quantity - 1] = value;
        uint64_t subsqrt_index = 1;
        uint64_t subsqrt = 2;
        for (subsqrt = 2; subsqrt * subsqrt <= value; ++subsqrt) {
            subsqrts_and_paired[subsqrt_index] = subsqrt;
            subsqrts_and_paired[subsqrts_and_paired_quantity - subsqrt_index - 1] = value / subsqrt;

            ++subsqrt_index;
        }

        subsqrts_and_paired.push_back(subsqrt - 1);  // trunc(sqrt(value)) is the last element
    }

    return std::move(subsqrts_and_paired);
}

/// \brief Gets index of value contained in vector that was formed by function get_subsqrts_and_paired
/// \param vector - reference to earlier mentioned vector
/// \param threshold - sqrt(+) mentioned in trait (*)
/// \param ceil - number for which vector with numbers that match (*) is calculated
/// \param value - value searched for in vector
/// \return index of value in vector
uint64_t GetCorrectIndex(const std::vector<uint64_t> &vector, uint64_t threshold, uint64_t ceil, uint64_t value) {
    return value <= threshold ? value - 1 : (vector.size() - 1) - ceil / value;
}

/// \brief Counts prime numbers quantity from 1 to ceil
/// \param ceil - upper border for counting prime numbers
/// \return quantity of prime numbers
uint64_t GetPrimesQuantity(uint64_t ceil) {
    std::vector<uint64_t> subsqrts_and_paired = GetSubsqrtsAndPaired(ceil);
    uint64_t size = subsqrts_and_paired.size() - 1;
    uint64_t trunc_sqrt = subsqrts_and_paired[size];  // last element is trunc(sqrt(ceil))

    std::vector<uint64_t> dp(size);
    for (uint64_t i = 0; i < size; ++i) {
        dp[i] = subsqrts_and_paired[i];
    }

    uint64_t values_used_in_sieving = 0;
    for (uint64_t subsqrt = 2; subsqrt * subsqrt <= ceil; ++subsqrt) {
        if (dp[GetCorrectIndex(subsqrts_and_paired, trunc_sqrt, ceil, subsqrt - 1)] !=
            dp[GetCorrectIndex(subsqrts_and_paired, trunc_sqrt, ceil, subsqrt)]) {
            ++values_used_in_sieving;

            for (uint64_t i = size; i > 0; --i) {
                if (subsqrts_and_paired[i - 1] < subsqrt * subsqrt) {
                    break;
                }

                dp[i - 1] -=
                    dp[GetCorrectIndex(subsqrts_and_paired, trunc_sqrt, ceil, subsqrts_and_paired[i - 1] / subsqrt)] -
                    values_used_in_sieving;
            }
        }
    }

    return dp[GetCorrectIndex(subsqrts_and_paired, trunc_sqrt, ceil, ceil)] - 1;
}

int main() {
    uint64_t n = 0;
    std::cin >> n;
    std::cout << GetPrimesQuantity(n);

    return 0;
}
