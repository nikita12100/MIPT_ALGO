#include <iostream>
#include <vector>
#include <map>

class node {
public:
    int parent;
    std::map<char, int> children;

    long long position;
    int left_idx, right_idx;
    int suff_link;
    int string_num;

    node(int l, int r, int par) : left_idx(l), right_idx(r), parent(par), suff_link(-1) {};

    node() : left_idx(0), right_idx(0), parent(-1), suff_link(-1) {};

    int &get_child(char character) {
        return (children.count(character) == 0) ? children[character] = -1 : children[character];
    }

    int length_substr() {
        return right_idx - left_idx;
    }
};

class condition {
public:
    int vertex, position;

    condition(int v, int pos) : vertex(v), position(pos) {}
};


struct generalized_suffix_tree {
public:
    generalized_suffix_tree(std::string &a, std::string &b) : size_str_a(a.size()), size_str_b(b.size()) {
        input = a + b;
        input_size = input.size();
        tree_nodes.resize(input.size());

        tree_size = 1;
        for (int i = 0; i < input_size; ++i) {
            expansion(i);
        }

    };

    void print();

private:
    std::vector <node> tree_nodes;
    condition pointer = condition(0, 0);
    int input_size;
    int tree_size;

    size_t size_str_a;
    size_t size_str_b;
    std::string input;
    int position = 0;

    void expansion(int current_pos);

    condition checker(condition st, int left, int right);

    int split(condition current_state);

    int get_suff_link(int vertex);

    void dfs_print(node *root);

    void dfs_convert(node *root);

};

void generalized_suffix_tree::print() {
    std::cout << tree_size << std::endl;

    dfs_convert(&tree_nodes[0]);
    dfs_print(&tree_nodes[0]);
}


void generalized_suffix_tree::expansion(int current_pos) {
    int mid = true;
    int current = 0;
    while (mid) {
        condition new_pointer = checker(pointer, current_pos, current_pos + 1);

        if (new_pointer.vertex != -1) {
            pointer = new_pointer;
            return;
        }

        mid = split(pointer);
        current = tree_size;
        tree_size++;

        if (tree_size >= tree_nodes.size()) {
            tree_nodes.resize(tree_nodes.size() * 2);
        }

        tree_nodes[current] = node(current_pos, input_size, mid);
        tree_nodes[mid].get_child(input[current_pos]) = current;

        pointer.vertex = get_suff_link(mid);
        pointer.position = tree_nodes[pointer.vertex].length_substr();

    }
}

condition generalized_suffix_tree::checker(condition state, int left, int right) {
    while (right > left) {
        if (state.position == tree_nodes[state.vertex].length_substr()) {
            state = condition(tree_nodes[state.vertex].get_child(input[left]), 0);

            if (state.vertex == -1) {
                return state;
            }

        } else if (state.position != tree_nodes[state.vertex].length_substr()) {

            if (input[tree_nodes[state.vertex].left_idx + state.position] != input[left]) {
                return condition(-1, -1);
            } else if (right - left < tree_nodes[state.vertex].length_substr() - state.position) {
                return condition(state.vertex, state.position - left + right);
            }

            left += tree_nodes[state.vertex].length_substr() - state.position;
            state.position = tree_nodes[state.vertex].length_substr();
        }
    }

    return state;
}

int generalized_suffix_tree::split(condition current_state) {
    if (current_state.position == tree_nodes[current_state.vertex].length_substr()) {
        return current_state.vertex;
    } else if (current_state.position == 0) {
        return tree_nodes[current_state.vertex].parent;
    }
    node current_vertex = tree_nodes[current_state.vertex];

    int currrent_idx = tree_size++;
    if (tree_size >= tree_nodes.size()) {
        tree_nodes.resize(tree_nodes.size() * 2);
    }

    int r_border = current_vertex.left_idx + current_state.position;

    tree_nodes[currrent_idx] = node(current_vertex.left_idx, r_border, current_vertex.parent);
    tree_nodes[current_vertex.parent].get_child(input[current_vertex.left_idx]) = currrent_idx;
    tree_nodes[currrent_idx].get_child(input[r_border]) = current_state.vertex;
    tree_nodes[current_state.vertex].parent = currrent_idx;
    tree_nodes[current_state.vertex].left_idx += current_state.position;

    return currrent_idx;
}

int generalized_suffix_tree::get_suff_link(int vertex) {
    if (tree_nodes[vertex].parent == -1) {
        return 0;
    } else if (tree_nodes[vertex].suff_link != -1) {
        return tree_nodes[vertex].suff_link;
    }

    int to = get_suff_link(tree_nodes[vertex].parent);

    condition temp = condition(to, tree_nodes[to].length_substr());
    int left = tree_nodes[vertex].left_idx + (tree_nodes[vertex].parent == 0);
    int right = tree_nodes[vertex].right_idx;

    return tree_nodes[vertex].suff_link = split(checker(temp, left, right));
}

void generalized_suffix_tree::dfs_convert(node *root) {
    root->position = position;

    if (root->left_idx >= size_str_a) {
        root->string_num = 1;
        root->left_idx -= size_str_a;
        root->right_idx -= size_str_a;

    } else if (root->right_idx >= size_str_a) {
        root->string_num = 0;
        root->right_idx -= size_str_b;

    } else {
        root->string_num = 0;
    }

    for (auto elem: root->children) {
        position++;
        dfs_convert(&tree_nodes[elem.second]);
    }
}

void generalized_suffix_tree::dfs_print(node *root) {
    if (root != &tree_nodes[0]) {
        printf("%lld %d %d %d\n",
               tree_nodes[root->parent].position,
               root->string_num,
               root->left_idx,
               root->right_idx
        );
    }

    for (auto elem: root->children) {
        dfs_print(&tree_nodes[elem.second]);
    }

}

int main() {
    std::string a;
    std::string b;
    std::cin >> a >> b;

    generalized_suffix_tree gen_tree(a, b);
    gen_tree.print();

    return 0;
}