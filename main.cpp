#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstddef>

template<typename T>
class BST {
private:
    class Node;

public:
    using value_type = T;
    using size_type = std::size_t;
    using node = Node;


    // CONSTRUCTORS & DESTRUCTOR

    BST() : root(nullptr) {};

    explicit BST(Node *_root) : root(_root) {};

    ~BST() { clear(); };


    // MEMBER FUNCTIONS

    void clear();
    // erases all nodes O(n)

    value_type size() const;
    // nodes amount O(1)

    value_type min() const;
    // min key O(h)

    value_type max() const;
    // max key O(h)

    value_type *find(const value_type &key) const;
    // pointer on key if is exists or nullptr

    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] bool contains(const value_type &key) const;
    // key existing flag

    [[nodiscard]] value_type *next(const value_type &key) const;
    // next value in no-decreasing order

    void insert(const value_type &key);
    // inserts node with key into BST

    void erase(const value_type &key);
    // erases node with key from BST

    void get_ordered(std::vector<value_type> &data) const;
    // fill array with keys in no-decreasing order


private:
    class Node {
    public:
        value_type key;
        Node *left = nullptr;
        Node *right = nullptr;
        Node *parent = nullptr;

        explicit Node(value_type key) : key(key) {}
    };

    size_type _size{0};
    Node *root{nullptr};

    Node *find_node(Node *curr_node, const value_type &key) const;
    // get node by key

    Node *next_node(const Node *node) const;
    // next node with value equal or bigger than in current node

    void get_ordered(std::vector<value_type> &data, Node *node) const;
    // fill vector with sorted elements

    void erase_node(Node *node);
    // erases node from BST

    void erase_subtree(Node *node);
    // erases subtree with particular root

};

template<typename T>
void BST<T>::clear() {
    erase_subtree(root);
}

template<typename T>
T BST<T>::size() const {
    return _size;
}

template<typename T>
T BST<T>::min() const {
    Node *curr = root;
    while (curr->left) {
        curr = curr->left;
    }
    return curr->key;
}

template<typename T>
T BST<T>::max() const {
    Node *curr = root;
    while (curr->right) {
        curr = curr->right;
    }
    return curr->key;
}

template<typename T>
T *BST<T>::find(const value_type &key) const {
    return nullptr;
}

template<typename T>
bool BST<T>::contains(const value_type &key) const {
    return find(key) != nullptr;
}

template<typename T>
T *BST<T>::next(const value_type &key) const {
    Node *node = find_node(root, key);
    if (node == nullptr) {
        std::stringstream error;
        error << "No value " << key << " in tree to find next!\n";
        throw std::runtime_error(error.str());
    }
    auto result_node = next_node(node);
    return (result_node ? &(result_node->key) : nullptr);
}

template<typename T>
void BST<T>::insert(const value_type &key) {
    Node *node = root;
    Node *key_node = new Node{key};
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

template<typename T>
void BST<T>::erase(const value_type &key) {
    Node *node = find_node(root, key);
    erase_node(node);
}

template<typename T>
void BST<T>::get_ordered(std::vector<value_type> &data) const {
    data.clear();
    data.reserve(_size);
    get_ordered(data, root);
}

template<typename T>
typename BST<T>::Node *BST<T>::find_node(BST::Node *curr_node, const value_type &key) const {
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

template<typename T>
typename BST<T>::Node *BST<T>::next_node(const BST::Node *node) const {
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
}

template<typename T>
void BST<T>::get_ordered(std::vector<value_type> &data, BST::Node *node) const {
    if (node) {
        get_ordered(data, node->left);
        data.push_back(node->key);
        get_ordered(data, node->right);
    }
}

template<typename T>
void BST<T>::erase_node(BST::Node *node) {
    if (!node || !root) {
        return;
    }

    if (node == root) {
        if (!node->left && !node->right) {
            delete node;
            root = nullptr;
            --_size;
            return;
        }

        if (node->left && !node->right) {
            root->key = root->left->key;
            erase_node(root->left);
            return;
        }

        if (!node->left && node->right) {
            root->key = root->right->key;
            erase_node(root->right);
            return;
        }

        Node *node_next = next_node(root);
        root->key = node_next->key;
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

template<typename T>
void BST<T>::erase_subtree(BST::Node *node) {
    if (node) {
        erase_subtree(node->left);
        erase_subtree(node->right);
        erase_node(node);
    }
}

template<typename T>
bool BST<T>::isEmpty() const {
    return size() == 0;
}


void test() {
    BST<int> tree;
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

}

int main() {
    test();
    return 0;
}