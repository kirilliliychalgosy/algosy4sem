#include <cassert>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

#define aboba std::cout << "aboba " << __LINE__ << std::endl;

struct Vertex {
    enum class Color { WHITE, GREY, BLACK };

    Vertex() : color_(Color::WHITE), pushed_neighbours_(false) {
    }

    Color color_ = Color::WHITE;
    bool pushed_neighbours_ = false;
    std::vector<int> adj_{};
};

class AhoCorasickDFA {
    static const int kAlphabetSize = 2;
    static const char kAlphabetFirstSymb = '0';

    struct Node {
        Node()
            : to_(std::vector<int>(kAlphabetSize, -1))
            , go_(std::vector<int>(kAlphabetSize, 0))
            , suf_link_(0)
            , is_terminal_(false) {
        }

        std::vector<int> to_;
        std::vector<int> go_;
        int suf_link_ = 0;
        bool is_terminal_ = false;
    };

public:
    void SetLinksAndGo() {
        for (int symb_index = 0; symb_index < kAlphabetSize; ++symb_index) {
            nodes_[0].go_[symb_index] = nodes_[0].to_[symb_index] == -1 ? 0 : nodes_[0].to_[symb_index];
        }
        nodes_[0].suf_link_ = 0;

        std::queue<int> already_processed_nodes;
        already_processed_nodes.push(0);

        while (!already_processed_nodes.empty()) {
            int cur_node = already_processed_nodes.front();
            already_processed_nodes.pop();

            for (int symb_index = 0; symb_index < kAlphabetSize; ++symb_index) {
                int cur_descendant = nodes_[cur_node].to_[symb_index];
                if (cur_descendant != -1) {
                    nodes_[cur_descendant].suf_link_ =
                        cur_node == 0 ? 0 : nodes_[nodes_[cur_node].suf_link_].go_[symb_index];

                    for (int descendant_symb_index = 0; descendant_symb_index < kAlphabetSize;
                         ++descendant_symb_index) {
                        nodes_[cur_descendant].go_[descendant_symb_index] =
                            nodes_[cur_descendant].to_[descendant_symb_index] == -1
                                ? nodes_[nodes_[cur_descendant].suf_link_].go_[descendant_symb_index]
                                : nodes_[cur_descendant].to_[descendant_symb_index];
                    }

                    already_processed_nodes.push(cur_descendant);
                }
            }
        }
    }

    void AddStr(const std::string &str) {
        if (nodes_.empty()) {
            nodes_.emplace_back();
        }

        int cur_node = 0;
        int str_size = static_cast<int>(str.size());
        for (int i = 0; i < str_size; ++i) {
            if (nodes_[cur_node].to_[str[i] - kAlphabetFirstSymb] == -1) {
                nodes_[cur_node].to_[str[i] - kAlphabetFirstSymb] = static_cast<int>(nodes_.size());
                nodes_.emplace_back();
            }

            cur_node = nodes_[cur_node].to_[str[i] - kAlphabetFirstSymb];
        }

        nodes_[cur_node].is_terminal_ = true;
    }

    void MarkAdditionalTerminals() {
        std::queue<int> traversal;
        traversal.push(0);

        while (!traversal.empty()) {
            int cur_node = traversal.front();
            traversal.pop();

            for (int symb_index = 0; symb_index < kAlphabetSize; ++symb_index) {
                int cur_descendant = nodes_[cur_node].to_[symb_index];
                if (cur_descendant != -1) {
                    if (nodes_[nodes_[cur_descendant].suf_link_].is_terminal_) {
                        nodes_[cur_descendant].is_terminal_ = true;
                    }

                    traversal.push(cur_descendant);
                }
            }
        }
    }

    std::vector<Vertex> GetNoTerminalGraph() {
        std::vector<Vertex> vertices(nodes_.size());
        int dfa_size = static_cast<int>(nodes_.size());
        for (int node_index = 1; node_index < dfa_size; ++node_index) {
            for (int symb_index = 0; symb_index < kAlphabetSize; ++symb_index) {
                int cur_descendant = nodes_[node_index].go_[symb_index];
                if ((cur_descendant != 0) && !nodes_[node_index].is_terminal_ && !nodes_[cur_descendant].is_terminal_) {
                    vertices[node_index].adj_.push_back(cur_descendant);
                }
            }
        }

        return vertices;
    }

private:
    std::vector<Node> nodes_;
};

bool FindCycleInDfa(std::vector<Vertex> &vertices) {
    std::stack<int> traversal;
    traversal.push(1);

    while (!traversal.empty()) {
        int cur_node = traversal.top();
        vertices[cur_node].color_ = Vertex::Color::GREY;

        if (!vertices[cur_node].pushed_neighbours_) {
            vertices[cur_node].pushed_neighbours_ = true;

            int adj_quantity = static_cast<int>(vertices[cur_node].adj_.size());
            for (int i = 0; i < adj_quantity; ++i) {
                if (vertices[vertices[cur_node].adj_[i]].color_ == Vertex::Color::WHITE) {
                    traversal.push(vertices[cur_node].adj_[i]);
                } else if (vertices[vertices[cur_node].adj_[i]].color_ == Vertex::Color::GREY) {
                    return true;
                }
            }
        } else {
            vertices[cur_node].color_ = Vertex::Color::BLACK;
            traversal.pop();
        }
    }

    return false;
}

int main() {
    int n = 0;
    std::cin >> n;

    AhoCorasickDFA viruses_map{};
    for (int i = 0; i < n; ++i) {
        std::string virus_code;
        std::cin >> virus_code;

        viruses_map.AddStr(virus_code);
    }

    viruses_map.SetLinksAndGo();
    viruses_map.MarkAdditionalTerminals();
    std::vector<Vertex> no_terminal_graph = viruses_map.GetNoTerminalGraph();
    bool answer = FindCycleInDfa(no_terminal_graph);

    if (answer) {
        std::cout << "TAK";
    } else {
        std::cout << "NIE";
    }

    return 0;
}

