#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstddef>

template<typename T>
class Tree {
public:
    using value_type = T;
    using size_type = std::size_t;

    ~Tree() {
        clear();
    }

    void clear() {
        erase_subtree(_root);
    }

    value_type size() const {
        return _size;
    } // nodes cnt

    value_type min() const {
        Node *curr = _root;
        while (curr->left) {
            curr = curr->left;
        }
        return curr->key;
    }

    value_type max() const {
        Node *curr = _root;
        while (curr->right) {
            curr = curr->right;
        }
        return curr->key;
    }

    value_type *find(const value_type &key) const {
        return &find_node(_root, key)->key;
    } // nullptr if no value found

    value_type *next(const value_type &key) const {
        Node *node = find_node(_root, key);
        if (node == nullptr) {
            std::stringstream error;
            error << "No value " << key << " in tree to find next!\n";
            throw std::runtime_error(error.str());
        }
        auto result_node = next_node(node);
        return (result_node ? &(result_node->key) : nullptr);
    } // next value by no-decreasing order, nullptr if no value found;

    void insert(const value_type &key) {
        Node *node = _root;
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
            this->_root = key_node;
        }
        ++this->_size;
    }

    void erase(const value_type &key) {
        Node *node = find_node(_root, key);
        erase_node(node);
    }

    void get_ordered(std::vector<value_type> &data) const {
        data.clear();
        data.reserve(_size);
        get_ordered(data, _root);
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
    Node *_root{nullptr};

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
        Node *curr_node = find_node(_root, node->key);
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
        if (!node || !_root) {
            return;
        }


        if (node == _root) {
            if (!node->left && !node->right) {
                delete node;
                _root = nullptr;
                --_size;
                return;
            }

            if (node->left && !node->right) {
                _root->key = _root->left->key;
                erase_node(_root->left);
                return;
            }

            if (!node->left && node->right) {
                _root->key = _root->right->key;
                erase_node(_root->right);
                return;
            }

            Node *node_next = next_node(_root);
            _root->key = node_next->key;
            erase_node(node_next);
            return;
        }

        if (!node->left && !node->right) { // no children
            (node->parent->left == node ? node->parent->left : node->parent->right) = nullptr;
            delete node;
            --_size;
            return;
        }

        if (node->right) { // only right child
            (node->parent->left == node ? node->parent->left : node->parent->right) = node->right;
            node->right->parent = node->parent;
            delete node;
            --_size;
            return;
        }

        if (node->left) { // only left child
            (node->parent->left == node ? node->parent->left : node->parent->right) = node->left;
            node->left->parent = node->parent;
            delete node;
            --_size;
            return;
        }

        Node *node_next = next_node(node);
        if (node_next) {
            node->key = node_next->key;
            erase_node(node_next);
        }

    }

    void erase_subtree(Node *node) {
        if (node) {
            erase_subtree(node->left);
            erase_subtree(node->right);
            erase_node(node);
        }
    }


};

int main() {
    Tree<int> tree;
    std::cout << "INSERT & FIND\n";
    tree.insert(0);
    tree.insert(1);
    tree.insert(-1);
    std::cout << "0 : " << (tree.find(0) == nullptr ? "nullptr" : std::to_string(*tree.find(0))) << "\n";
    std::cout << "1 : " << (tree.find(1) == nullptr ? "nullptr" : std::to_string(*tree.find(1))) << "\n";
    std::cout << "-1 : " << (tree.find(-1) == nullptr ? "nullptr" : std::to_string(*tree.find(-1))) << "\n";
    std::cout << "nullptr : " << (tree.find(2) == nullptr ? "nullptr" : std::to_string(*tree.find(2))) << "\n";
    std::cout << "\n";

    std::cout << "MIN & MAX\n";
    std::cout << "-1 : " << tree.min() << "\n";
    std::cout << "1 : " << tree.max() << "\n";
    std::cout << "\n";

    std::cout << "ERASE\n";
    tree.erase(1);
    tree.erase(0);
    std::cout << "nullptr : " << (tree.find(0) == nullptr ? "nullptr" : std::to_string(*tree.find(0))) << "\n";
    std::cout << "1 : " << tree.size() << "\n";

    std::cout << "\n";
    std::cout << "CLEAR\n";
    tree.insert(10);
    tree.insert(-10);
    tree.clear();
    std::cout << "0 : " << tree.size();

    return 0;
}