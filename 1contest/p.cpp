#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <stack>
#include <vector>


struct RefrainContext
{
    uint64_t enterings_quantity_ = 0;
    uint64_t size_               = 0;
    uint64_t index_              = 0;
};


struct SuffixContext
{
    uint64_t initial_index = 0;
    uint64_t rank          = 0;
    uint64_t next_rank     = 0;
};

bool Comparator(const SuffixContext &suf1, const SuffixContext &suf2)
{
    return suf1.rank < suf2.rank ? true : suf1.rank == suf2.rank ? suf1.next_rank < suf2.next_rank : false;
}

std::vector<uint64_t> BuildSuffArr(const std::string &str)
{
    uint64_t str_size = str.size();
    std::vector<SuffixContext> suffixes(str_size);
 
    for (uint64_t suffix_index = 0; suffix_index < str_size; ++suffix_index)
    {
        suffixes[suffix_index].initial_index = suffix_index;
        suffixes[suffix_index].rank      = str[suffix_index];
        suffixes[suffix_index].next_rank = suffix_index == str_size - 1 ? -1 : str[(suffix_index + 1)];
    }

    std::sort(suffixes.begin(), suffixes.end(), Comparator);

    for (uint64_t sorted_length = 2; sorted_length < str_size; sorted_length *= 2)
    {
        uint64_t prev_rank = suffixes[0].rank;
        suffixes[0].rank = 0;
 
        std::vector<uint64_t> intermediary_order(str_size);
        intermediary_order[suffixes[0].initial_index] = 0;
        uint64_t cur_rank = 0;
        for (uint64_t suffix_index = 1; suffix_index < str_size; ++suffix_index)
        {   
            bool equal_to_prev = false;

            if ((suffixes[suffix_index].rank == prev_rank) &&
                (suffixes[suffix_index].next_rank == suffixes[suffix_index - 1].next_rank))
            {
                equal_to_prev = true;
            }

            prev_rank = suffixes[suffix_index].rank;
            if (!equal_to_prev)
            {
                ++cur_rank;
            }
            suffixes[suffix_index].rank = cur_rank;

            intermediary_order[suffixes[suffix_index].initial_index] = suffix_index;
        }

        for (uint64_t suffix_index = 0; suffix_index < str_size; ++suffix_index)
        {
            uint64_t next_suffix_real_index = suffixes[suffix_index].initial_index + sorted_length;
            suffixes[suffix_index].next_rank = next_suffix_real_index >= str_size ? -1 : suffixes[intermediary_order[next_suffix_real_index]].rank;
        }
 
        std::sort(suffixes.begin(), suffixes.end(), Comparator);
    }
 
    std::vector<uint64_t> result(str_size);
    for (uint64_t i = 0; i < str_size; ++i)
    {
        result[i] = suffixes[i].initial_index + 1;
    }

    return result;
}

std::vector<uint64_t> KasaiArimuraArikawaLeePark(const std::vector<uint64_t> &suf_arr, const std::string &str)
{
    uint64_t suf_arr_size = suf_arr.size();
    std::vector<uint64_t> inversed_suf_arr(suf_arr_size);
    for (uint64_t suf_index_alph_order = 0; suf_index_alph_order < suf_arr_size; ++suf_index_alph_order)
    {
        inversed_suf_arr[suf_arr[suf_index_alph_order] - 1] = suf_index_alph_order;
    }
    
    uint64_t cur_lcp_size = 0;
    std::vector<uint64_t> lcp(suf_arr_size);
    for (uint64_t suf_index = 0; suf_index < suf_arr_size; ++suf_index)
    {
        if (inversed_suf_arr[suf_index] == suf_arr_size - 1)
        {
            cur_lcp_size = 0;
        }
        else
        {
            uint64_t neighbour = suf_arr[inversed_suf_arr[suf_index] + 1] - 1;
            while ((suf_index + cur_lcp_size < suf_arr_size) &&
                   (neighbour + cur_lcp_size < suf_arr_size) &&
                   (str[suf_index + cur_lcp_size] == str[neighbour + cur_lcp_size]))
            {
                ++cur_lcp_size;
            }
        }

        lcp[inversed_suf_arr[suf_index]] = cur_lcp_size;

        if (cur_lcp_size > 0)
        {
            --cur_lcp_size;
        }
    }

    return lcp;
}

RefrainContext FindBiggestRefrain(const std::string &str)
{
    std::vector<uint64_t> suff_arr = BuildSuffArr(str);
    std::vector<uint64_t> lcp      = KasaiArimuraArikawaLeePark(suff_arr, str);

    uint64_t str_size = str.size();
    std::stack<RefrainContext> stack;

    RefrainContext max_refrain{1, str_size, 0};

    for (uint64_t pos = 0; pos < str_size; ++pos)
    {
        uint64_t cur_refrain_enterings_quantity = 1;

        while (!stack.empty() && (lcp[pos] <= stack.top().size_))
        {
            RefrainContext largest_prev = stack.top();
            stack.pop();

            cur_refrain_enterings_quantity += largest_prev.enterings_quantity_;
            if (cur_refrain_enterings_quantity  * largest_prev.size_ >=
                max_refrain.enterings_quantity_ * max_refrain.size_)
            {
                max_refrain = {cur_refrain_enterings_quantity, 
                               largest_prev.size_,
                               suff_arr[largest_prev.index_] - 1};
            }
        }

        if (stack.empty() || (lcp[pos] > stack.top().size_))
        {
            stack.push({cur_refrain_enterings_quantity, lcp[pos], pos});
        }
    }

    return max_refrain;
}

 
int main()
{
    uint64_t n = 0;
    uint64_t m = 0;
    std::cin >> n >> m;
    
    std::string input;
    for (uint64_t i = 0; i < n; ++i)
    {
        uint64_t value = 0;
        std::cin >> value;
        input.push_back(static_cast<char> (value));
    }

    RefrainContext max_refrain = FindBiggestRefrain(input);
    std::cout << max_refrain.enterings_quantity_ * max_refrain.size_ << std::endl
              << max_refrain.size_                                   << std::endl;
    for (uint64_t i = max_refrain.index_; i < max_refrain.index_ + max_refrain.size_; ++i)
    {
        std::cout << static_cast<int> (input[i]) << " ";
    }

    return 0;
}
