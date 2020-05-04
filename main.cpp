#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstddef>a

template<typename T>
class Tree {
public:
    using value_type = T;
    using size_type = std::size_t;

    value_type size() const {
        return _size;
    } // nodes cnt

    value_type min() const {
        Node *curr = root;
        while (curr->left) {
            curr = curr->left;
        }
        return curr->key;
    }

    value_type max() const {
        Node *curr = root;
        while (curr->right) {
            curr = curr->right;
        }
        return curr->key;
    }

    value_type *find(const value_type &key) const {
        return &find_node(root, key)->key;
    } // nullptr if no value found

    value_type *next(const value_type &key) const {
        Node *node = find_node(root, key);
        if (node == nullptr) {
            std::stringstream error;
            error << "No value " << key << " in tree to find next!\n";
            throw std::runtime_error(error.str());
        }
        auto result_node = next_node(node);
        return (result_node ? &(result_node->key) : nullptr);
    } // next value by no-decreasing order, nullptr if no value found;

    void insert(const value_type &key) {
        Node *node = root;
        Node *key_node = new Node(key);
        while (node) {
            key_node->parent = node;
            if (node->key < key) {
                node = node->right;
            } else {
                node = node->left;
            }
        }
        if (key_node->parent) {
            if (key_node->parent->key < key_node->key) {
                key_node->parent->right = key_node;
            } else {
                key_node->parent->left = key_node;
            }
        } else {
            this->root = key_node;
        }
        ++this->_size;
    }

    void erase(const value_type &key) {
        Node *node = find_node(key);
        erase_node(node);
    }

    void get_ordered(std::vector<value_type> &data) const {
        data.clear();
        data.reserve(_size);
        get_ordered(data, root);
    }


private:
    class Node {
    public:
        int key;
        Node *parent{nullptr};
        Node *left{nullptr};
        Node *right{nullptr};

        explicit Node(const int key,
                      Node *p = nullptr,
                      Node *left = nullptr,
                      Node *right = nullptr) :
                key(key),
                parent(p), left(left),
                right(right) {
        }

    };

    size_type _size{0};
    Node *root{nullptr};

    Node *find_node(Node *curr_node, const value_type &key) const {
        if (!curr_node) {
            return nullptr;
        }
        if (curr_node->key > key) {
            return find_node(curr_node->left, key);
        }
        if (curr_node->key < key) {
            return find_node(curr_node->right, key);
        }
        return curr_node;
    }

    Node *next_node(const Node *node) const {
        Node *curr_node = find_node(root, node->key);
        if (curr_node->right) {
            curr_node = curr_node->right;
            while (curr_node->left) {
                curr_node = curr_node->left;
            }
            return curr_node;
        }
        while (curr_node->parent && curr_node->parent->right == curr_node) {
            curr_node = curr_node->parent;
        }
        return (curr_node->parent ? curr_node->parent : nullptr);
    } // next node with value equal or bigger than in current node

    void get_ordered(std::vector<value_type> &data, Node *node) const {
        if (node) {
            get_ordered(data, node->left);
            data.push_back(node->key);
            get_ordered(data, node->right);
        }
    } // fill vector with sorted elements

    void erase_node(Node *node) {
        if (!(node->left) || node->right) {
            delete node;
            return;
        }
        Node *replace_node;
        if (node->left && node->right) {
            replace_node = next_node(node);
            if (replace_node->right) {
                if (replace_node->parent->left == replace_node) {
                    replace_node->parent->left = replace_node->right;
                } else {
                    replace_node->parent->right = replace_node->right;
                }
            } else {
                if (replace_node->parent->left == replace_node) {
                    replace_node->parent->left = nullptr;
                } else {
                    replace_node->parent->right = nullptr;
                }
            }
        } else if (node->right) {
            replace_node = node->right;
        } else {
            replace_node = node->left;
        }
        if (node->parent->left == node) {
            node->parent->left = replace_node;
        } else {
            node->parent->right = replace_node;
        }
        replace_node->parent = node->parent;
        delete node;
    } // не разобарлся как это иначе написать...

};

int main() {
    Tree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(0);
    tree.insert(-3);

    std::cout << "find\n1 : ";
    std::cout << *tree.find(1) << "\n";

    std::cout << "max\n2 : ";
    std::cout << tree.max() << "\n";

    std::cout << "min\n-3 : ";
    std::cout << tree.min() << "\n";

    std::cout << "ordered\n-3 0 1 2 : ";
    std::vector<int> data;
    tree.get_ordered(data);
    for (const auto &elem : data) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    std::cout << "next\n" << "1 : " << (tree.next(0) == nullptr ? "nullptr" : std::to_string(*tree.next(0))) << "\n";
    return 0;
}