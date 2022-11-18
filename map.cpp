#include <iostream>
#include <algorithm>

template <typename Key, typename Value>
class map
{
    struct Node
    {
        Key key;
        Value data;
        Node *leftChild;
        Node *rightChild;
        template <typename K, typename V>
        Node(K &&k, V &&v) : key{std::forward<K>(k)}, data{std::forward<V>(v)}, leftChild{nullptr}, rightChild{nullptr} {}
    };
    size_t size_;
    Node *root = nullptr;

    void clear(Node *node)
    {
        if (!node)
            return;
        clear(node->leftChild);
        clear(node->rightChild);
        delete node;
    }
    void printNode(Node *node)
    {
        if (!node)
            return;
        printNode(node->leftChild);
        std::cout << node->key << "    " << node->data << std::endl;
        printNode(node->rightChild);
    }
    void addNode(Node *&node, const std::pair<Key, Value> &pair)
    {
        if (!node)
        {
            node = new Node(pair.first, pair.second);
            ++size_;
            return;
        }
        if (node->key == pair.first)
        {
            node->data = pair.second;
            return;
        }
        if (pair.first < node->key)
            addNode(node->leftChild, pair);
        else
            addNode(node->rightChild, pair);
    }
    Value *findNode(Node *node, const Key &key)
    {
        if (!node)
            return nullptr;
        if (node->key == key)
            return &node->data;
        if (key < node->key)
            return findNode(node->leftChild, key);
        else
            return findNode(node->rightChild, key);
    }
    void f(Node *node)
    {
        if (!node)
            return;
        addNode(root, std::make_pair(node->key, node->data));
        f(node->leftChild);
        f(node->rightChild);
    }

public:
    map() : size_{0}, root{nullptr} {}
    map(const map &second)
    {
        size_ = second.size_;
        f(second.root);
    }
    map(map &&second)
    {
        size_ = second.size_;
        root = second.root;
        second.size_ = 0;
        second.root = nullptr;
    }
    map &operator=(const map &second)
    {
        if (&second == this)
            return *this;
        clear(root);
        size_ = second.size_;
        f(second.root);
        return *this;
    }
    map &operator=(map &&second)
    {
        if (&second == this)
            return *this;
        clear(root);
        size_ = second.size_;
        root = second.root;
        second.size_ = 0;
        second.root = nullptr;
        return *this;
    }
    ~map()
    {
        clear(root);
        root = nullptr;
        size_ = 0;
    }
    size_t size() { return size_; }
    bool empty() { return size_ == 0; }
    Value &operator[](const Key &key)
    {
        auto found = findNode(root, key);
        if (found)
            return *found;
        else
        {
            insert(std::make_pair(key, Value{}));
            return *(findNode(root, key));
        }
    }
    Value &find(const Key &key)
    {
        auto found = findNode(root, key);
        if (found)
            return *found;
        else
            throw std::logic_error("Ne postoji");
    }
    void insert(std::pair<Key, Value> pair)
    {
        addNode(root, pair);
    }
    void print()
    {
        printNode(root);
        std::cout << std::endl;
    }
    void erase(const Key &key)
    {
        if (empty())
            throw std::logic_error("Empty");
        if (root->key == key)
        {
            if (root->leftChild && !root->rightChild)
            {
                Node *temp = root;
                root = root->leftChild;
                delete temp;
                --size_;
                return;
            }
            else if (!root->leftChild && root->rightChild)
            {
                Node *temp = root;
                root = root->rightChild;
                delete temp;
                --size_;
                return;
            }
            else if (!root->leftChild && !root->rightChild)
            {
                delete root;
                root = nullptr;
                size_ = 0;
                return;
            }
            else if (root->leftChild && root->rightChild)
            {
                Node *temp = root->rightChild;
                Node *parent = root;
                while (temp->leftChild)
                {
                    parent = temp;
                    temp = temp->leftChild;
                }
                std::swap(root->data, temp->data);
                std::swap(root->key, temp->key);
                if (root != parent)
                {
                    if (temp->rightChild)
                        parent->leftChild = temp->rightChild;
                    else
                        parent->leftChild = nullptr;
                }
                else
                    root->rightChild = nullptr;
                delete temp;
                --size_;
                return;
            }
        }
        Node *current = root;
        Node *parent = root;
        while (current)
        {
            if (key == current->key)
                break;
            parent = current;
            if (key < current->key)
                current = current->leftChild;
            else
                current = current->rightChild;
        }
        if (!current)
            return;
        if (!current->leftChild && !current->rightChild)
        {
            if (current == parent->leftChild)
                parent->leftChild = nullptr;
            else
                parent->rightChild = nullptr;
        }
        else if (current->leftChild && !current->rightChild)
        {
            if (parent->leftChild == current)
                parent->leftChild = current->leftChild;
            else
                parent->rightChild = current->leftChild;
        }
        else if (!current->leftChild && current->rightChild)
        {
            if (parent->leftChild == current)
                parent->leftChild = current->rightChild;
            else
                parent->rightChild = current->rightChild;
        }
        else if (current->leftChild && current->rightChild)
        {
            Node *temp = current->rightChild;
            while (temp->leftChild)
            {
                parent = temp;
                temp = temp->leftChild;
            }
            std::swap(current->data, temp->data);
            std::swap(current->key, temp->key);

            if (temp->rightChild)
                parent->leftChild = current->rightChild;
            else
                parent->leftChild = nullptr;
            current = temp;
        }
        delete current;
        --size_;
        return;
    }
};

int main(int argc, char const *argv[])
{
   
}
