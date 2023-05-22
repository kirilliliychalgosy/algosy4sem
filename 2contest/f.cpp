#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

/*! \class PrimesQuantity
    \brief Class that counts quantity of prime numbers from 2 to argument ceil in constructor

    \param ceil_ - value until for which all is being calculated
    \param subsqrts_and_paired_ - vector which contains all numbers(called subsqrts) from 2 to trunc(sqrt(ceil_))
                                  and numbers that are received by doing ceil_ / subsqrt for each of subsqrt number
    Class goes through subsqrts_and_paired vector, counting prime numbers quantity
*/
class PrimesQuantity {
public:
    explicit PrimesQuantity(uint64_t ceil) : ceil_(ceil) {
    }

    /// \brief Counts prime numbers quantity from 1 to ceil
    /// \param ceil - upper border for counting prime numbers
    /// \return quantity of prime numbers
    uint64_t GetPrimesQuantity() {
        subsqrts_and_paired_ = GetSubsqrtsAndPaired(ceil_);
        auto subsqrts_and_paired_begin = subsqrts_and_paired_.cbegin();
        auto subsqrts_and_paired_end = subsqrts_and_paired_.end() - 1;  // last element is trunc(sqrt(ceil))
        auto subsqrts_and_paired_iterator = subsqrts_and_paired_begin;
        uint64_t trunc_sqrt = *subsqrts_and_paired_end;

        std::vector<uint64_t> dp(subsqrts_and_paired_end - subsqrts_and_paired_begin);
        auto dp_begin = dp.begin();
        auto dp_end = dp.end();
        auto dp_iterator = dp_begin;
        while (dp_iterator != dp_end) {
            *dp_iterator = *subsqrts_and_paired_iterator;

            ++dp_iterator;
            ++subsqrts_and_paired_iterator;
        }

        uint64_t values_used_in_sieving = 0;
        for (uint64_t subsqrt = 2; subsqrt * subsqrt <= ceil_; ++subsqrt) {

            if (*(dp_begin + static_cast<std::vector<uint64_t>::difference_type>(
                                 GetCorrectIndex(subsqrts_and_paired_, trunc_sqrt, ceil_, subsqrt - 1))) !=
                *(dp_begin + static_cast<std::vector<uint64_t>::difference_type>(
                                 GetCorrectIndex(subsqrts_and_paired_, trunc_sqrt, ceil_, subsqrt)))) {
                ++values_used_in_sieving;

                subsqrts_and_paired_iterator = subsqrts_and_paired_end - 1;
                dp_iterator = dp_end - 1;
                while (subsqrts_and_paired_iterator != subsqrts_and_paired_begin) {
                    if (*subsqrts_and_paired_iterator < subsqrt * subsqrt) {
                        break;
                    }

                    *dp_iterator -= *(dp.begin() + static_cast<std::vector<uint64_t>::difference_type>(
                                                       GetCorrectIndex(subsqrts_and_paired_, trunc_sqrt, ceil_,
                                                                        *subsqrts_and_paired_iterator / subsqrt))) -
                                    values_used_in_sieving;

                    --subsqrts_and_paired_iterator;
                    --dp_iterator;
                }
            }
        }

        return *(dp_begin + static_cast<std::vector<uint64_t>::difference_type>(
                                GetCorrectIndex(subsqrts_and_paired_, trunc_sqrt, ceil_, ceil_))) -
               1;
    }

private:     
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
        auto subsqrts_and_paired_begin = subsqrts_and_paired.begin();
        auto subsqrts_and_paired_end = subsqrts_and_paired.end();
        *subsqrts_and_paired_begin = 1;

        if (subsqrts_and_paired_quantity != 1) {
            *(subsqrts_and_paired_end - 1) = value;
            auto subsqrts_and_paired_iterator = subsqrts_and_paired_begin + 1;
            uint64_t subsqrt = 2;
            for (subsqrt = 2; subsqrt * subsqrt <= value; ++subsqrt) {
                *subsqrts_and_paired_iterator = subsqrt;
                *(subsqrts_and_paired_begin + static_cast<std::vector<uint64_t>::difference_type>(
                                                  subsqrts_and_paired_end - subsqrts_and_paired_iterator - 1)) =
                    value / subsqrt;

                ++subsqrts_and_paired_iterator;
            }

            subsqrts_and_paired.push_back(subsqrt - 1);  // trunc(sqrt(value)) is the last element
        }

        return subsqrts_and_paired;
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

private:
    //-----------------------------------Variables-------------------------------------
    uint64_t ceil_;
    std::vector<uint64_t> subsqrts_and_paired_;
};

int main() {
    uint64_t n = 0;
    std::cin >> n;
    PrimesQuantity result(n);
    std::cout << result.GetPrimesQuantity();

    return 0;
}
