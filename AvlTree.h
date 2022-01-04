
#ifndef WET2_AVLTREE_H
#define WET2_AVLTREE_H


#include "memory"
#include "Utils.h"
#include "SimpleArray.h"

using namespace std;


class TreeEmpty : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "Can not do this because the tree is empty";
    }
};

class KeyExists : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "Can not do this because this key is already in the tree";
    }
};

class KeyNotExist : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "Can not do this because this key is not in the tree";
    }
};

class InvalidRank : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "The requested rank is not in the range of possible ranks for this tree";
    }
};

class InvalidIndexInput : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "the input index does not make sense";
    }
};

template<class K, class T>
struct TNode {
    typedef TNode<K,T> _node;
    typedef shared_ptr<_node> _node_ptr;

    K key;
    T data;
    _node_ptr left;
    _node_ptr right;
    int height;
    int size;

    TNode(const K& key, T data) :
            key(key),
            data(data),
            left(nullptr),
            right(nullptr),
            height(0),
            size(1)
    {}

    TNode(const _node& other) :
            key(other.key),
            data(other.data),
            left(other.left),
            right(other.right),
            height(other.height),
            size(other.size)
    {}
};

template<class K, class T>
class AVLTree {
    typedef TNode<K,T> _node;
    typedef shared_ptr<_node> _node_ptr;

private:
    _node_ptr root;
    int size;

    static const _node_ptr& _find(const _node_ptr& root, const K& key){
        if(root == nullptr || key == root->key){
            return root;
        }

        if(key < root->key){
            return AVLTree<K,T>::_find(root->left, key);
        }

        return AVLTree<K,T>::_find(root->right, key);
    }

    static void _insert(_node_ptr& root, const K& key, const T& data) {
        if(root == nullptr){
            root = make_shared<_node>(key, data);
            return;
        }

        if(key == root->key){
            throw KeyExists();
        }

        if(key < root->key){
            AVLTree<K,T>::_insert(root->left, key, data);
            root->size = AVLTree<K,T>::_size(root->left) + AVLTree<K,T>::_size(root->right) + 1;
        }

        else {
            AVLTree<K,T>::_insert(root->right, key, data);
            root->size = AVLTree<K,T>::_size(root->left) + AVLTree<K,T>::_size(root->right) + 1;
        }

        AVLTree<K,T>::_balance(root);
        root->height = Utils::max(AVLTree<K,T>::_height(root->left), AVLTree<K,T>::_height(root->right)) + 1;
        root->size = AVLTree<K,T>::_size(root->left) + AVLTree<K,T>::_size(root->right) + 1;

    }

    static int _height(_node_ptr& root){
        return !root ? -1 : root->height;
    }

//        static int _calc_height(_node_ptr& root){
//            return !root ?
//            -1 :
//            1 + Utils::max(AVLTree<K,T>::_calc_height(root->left), AVLTree<K,T>::_calc_height(root->right));
//        }

    static int _size(_node_ptr& root){
        return !root ? 0 : root->size;
    }

//        static int _calc_size(_node_ptr& root){
//            return !root ? 0 : 1 + AVLTree<K,T>::_calc_size(root->left) + AVLTree<K,T>::_calc_size(root->right);
//        }

    static int _BF(const _node_ptr& root) {
        return AVLTree<K,T>::_height(root->left) - AVLTree<K,T>::_height(root->right);
    }

    static void _balance(_node_ptr& root) {
        if(!root) {
            return;
        }

        int BF = AVLTree<K,T>::_BF(root);
        if(BF == 2) {
            if(AVLTree<K,T>::_BF(root->left) >= 0) {
                AVLTree<K,T>::LL(root);
            }
            else if (AVLTree<K,T>::_BF(root->left) == -1) {
                AVLTree<K,T>::LR(root);
            }
        }

        else if(BF == -2) {
            if(AVLTree<K,T>::_BF(root->right) <= 0) {
                AVLTree<K,T>::RR(root);
            }
            else if(AVLTree<K,T>::_BF(root->right) == 1) {
                AVLTree<K,T>::RL(root);
            }
        }
    }

    static void RR(_node_ptr& root) {
        _node_ptr child = root->right;
        root->right = child->left;

        root->height = Utils::max(AVLTree<K,T>::_height(root->left), AVLTree<K,T>::_height(root->right)) + 1;
        child->height = Utils::max(AVLTree<K,T>::_height(child->right), AVLTree<K,T>::_height(root)) + 1;

        child->left = root;
        root = child;

        AVLTree<K,T>::_update_size(root);
        AVLTree<K,T>::_update_size(child);
    }

    static void LL(_node_ptr& root)
    {
        _node_ptr child = root->left;
        _node_ptr tmp = child->right;
        child->right = root;
        root->left = tmp;

        root->height = Utils::max(AVLTree<K,T>::_height(root->left), AVLTree<K,T>::_height(root->right)) + 1;
        child->height = Utils::max(AVLTree<K,T>::_height(child->left), AVLTree<K,T>::_height(child->right)) + 1;

        root = child;

        AVLTree<K,T>::_update_size(root);
        AVLTree<K,T>::_update_size(child);
    }

    static void RL(_node_ptr& root) {
        AVLTree<K,T>::LL(root->right);
        AVLTree<K,T>::RR(root);
    }

    static void LR(_node_ptr& root) {
        AVLTree<K,T>::RR(root->left);
        AVLTree<K,T>::LL(root);
    }

    static void _remove(_node_ptr& root, K key) {
        if(!root) {
            return;
        }

        if(key < root->key) {
            AVLTree<K,T>::_remove(root->left, key);
            if(root){
                root->height = Utils::max(AVLTree<K,T>::_height(root->left), AVLTree<K,T>::_height(root->right)) + 1;
                root->size = AVLTree<K,T>::_size(root->left) + AVLTree<K,T>::_size(root->right) + 1;
            }
        }

        else if(root->key < key) {
            AVLTree<K,T>::_remove(root->right, key);
            if(root){
                root->height = Utils::max(AVLTree<K,T>::_height(root->left), AVLTree<K,T>::_height(root->right));
                root->size = AVLTree<K,T>::_size(root->left) + AVLTree<K,T>::_size(root->right) + 1;
            }
        }

        else {
            if(AVLTree<K,T>::_is_leaf(root)) {
                root.reset();
                root = nullptr;
            }

            else if(root->left && !root->right) {
                root = root->left;
            }

            else if(root->right && !root->left) {
                root = root->right;
            }

            else {
                _node_ptr next = root->right;

                while(next->left) {
                    next = next->left;
                }

                root->key = next->key;
                root->data = next->data;

                AVLTree<K,T>::_remove(root->right, root->key);
                root->height = Utils::max(AVLTree<K,T>::_height(root->left), AVLTree<K,T>::_height(root->right)) + 1;
                root->size = AVLTree<K,T>::_size(root->left) + AVLTree<K,T>::_size(root->right) + 1;
            }
        }

        AVLTree<K,T>::_balance(root);
        if(root){
            root->height = Utils::max(AVLTree<K,T>::_height(root->left), AVLTree<K,T>::_height(root->right)) + 1;
            root->size = AVLTree<K,T>::_size(root->left) + AVLTree<K,T>::_size(root->right) + 1;
        }
    }

    static bool _is_leaf(_node_ptr& root){
        return !(root->left || root->right);
    }

    static int _rank(_node_ptr& root, K key, int count_smaller=0){
        if(!root){
            throw KeyNotExist();
        }

        if(key < root->key){
            return AVLTree<K,T>::_rank(root->left, key, count_smaller);
        }

        if(root->key < key){
            return AVLTree<K,T>::_rank(root->right,
                                       key,
                                       count_smaller + 1 + AVLTree<K,T>::_size(root->left));
        }

        else {
            return count_smaller + 1 + AVLTree<K,T>::_size(root->left);
        }
    }

    static K& _select(_node_ptr& root, int rank){
        if(root == nullptr){
            throw TreeEmpty();
        }

        if(rank <= 0 || rank > AVLTree<K,T>::_size(root)){
            throw InvalidRank();
        }

        int left_size = AVLTree<K,T>::_size(root->left);

        if(rank <= left_size){
            return AVLTree<K,T>::_select(root->left, rank);
        }

        else if(rank == left_size + 1){
            return root->key;
        }

        else {
            return AVLTree<K,T>::_select(root->right, rank - left_size - 1);
        }
    }

    static int _update_size(_node_ptr& root){
        if(!root){
            return 0;
        }

        root->size = AVLTree<K,T>::_update_size(root->left) + AVLTree<K,T>::_update_size(root->right) + 1;
        return root->size;
    }

    static int _update_height(_node_ptr& root){
        if(!root){
            return -1;
        }

        root->height = Utils::max(AVLTree<K,T>::_update_height(root->left),
                                  AVLTree<K,T>::_update_height(root->right)) + 1;
        return root->height;
    }

    static void _clean(_node_ptr& root){
        if(root){
            AVLTree<K,T>::_clean(root->left);
            AVLTree<K,T>::_clean(root->right);
            root.reset();
            root = nullptr;
        }
    }

    static void _print(_node_ptr& root, bool keys=true, bool data=false, int space=0) {
        if (!root)
            return;

        space += 10;

        if(root->right){
            AVLTree<K,T>::_print(root->right, keys, data, space);
        }

        cout << endl;
        for (int i = 10; i < space; i++){
            cout << " ";
        }

        if(keys) cout << root->key;
        cout << ":";
        if(data) cout << root->data;

        cout << "(s:" << root->size << "," << "h:" << root->height << ")";

        cout << "\n";

        if(root->left){
            AVLTree<K,T>::_print(root->left, keys, data, space);
        }
    }
    static int _ltq_key_count(_node_ptr& root, K key) {
        if(!root){
            return 0;
        }

        if(key < root->key){
            return AVLTree<K,T>::_ltq_key_count(root->left, key);
        }

        int left_size = AVLTree<K,T>::_size(root->left);

        if(root->key < key){
            return AVLTree<K,T>::_ltq_key_count(root->right, key) + left_size + 1;
        }

        else {
            return left_size + 1;
        }
    }
    static int _range_count(_node_ptr& root, K lower_bound, K upper_bound, int count=0){
        if(!root){
            return 0;
        }

        int ltq_low = AVLTree<K,T>::_ltq_key_count(root, lower_bound);
        int ltq_up = AVLTree<K,T>::_ltq_key_count(root, upper_bound);

        bool include_lower = AVLTree<K,T>::_find(root, lower_bound) != nullptr;

        return ltq_up - ltq_low + (include_lower ? 1 : 0);
    }

    static _node_ptr _merge(_node_ptr& root1, _node_ptr& root2){
        int size1 = AVLTree<K,T>::_size(root1);
        int size2 = AVLTree<K,T>::_size(root2);

        SimpleArray<_node_ptr> arr1(size1);
        SimpleArray<_node_ptr> arr2(size2);
        SimpleArray<_node_ptr> arr3(size1 + size2);

        AVLTree<K,T>::_to_sorted_array(root1, arr1, 0, size1);
        AVLTree<K,T>::_to_sorted_array(root2, arr2, 0, size2);

        AVLTree<K,T>::_merge_arrays(arr3, arr1, arr2, size1, size2);


        return AVLTree<K,T>::_array_to_tree(arr3, 0, size1 + size2 - 1);
    }

    static void _to_sorted_array(_node_ptr& root, SimpleArray<_node_ptr>& arr, int start_idx, int size){
        if(root && size > 0){
            int left_size = AVLTree<K,T>::_size(root->left);
            AVLTree<K,T>::_to_sorted_array(root->left, arr, start_idx, left_size);

            int right_size = AVLTree<K,T>::_size(root->right);
            AVLTree<K,T>::_to_sorted_array(root->right, arr, start_idx + left_size + 1, right_size);


            root->left.reset();
            root->right.reset();
            arr.set(start_idx + left_size, root);

        }
    }

    static void _merge_arrays(SimpleArray<_node_ptr>& dist, SimpleArray<_node_ptr>& arr1, SimpleArray<_node_ptr>& arr2, int size1, int size2){
//            auto arr3 = new SimpleArray<_node_ptr>(size1 + size2);
        int idx1 = 0, idx2 = 0, idx3 = 0;

        while(idx1 < size1 && idx2 < size2){
            if(arr1[idx1]->key < arr2[idx2]->key)
                dist.set(idx3++, arr1[idx1++]);
            else
                dist.set(idx3++, arr2[idx2++]);
        }
        while(idx1 < size1)
            dist.set(idx3++, arr1[idx1++]);
        while(idx2 < size2)
            dist.set(idx3++, arr2[idx2++]);
    }

    static _node_ptr _array_to_tree(SimpleArray<_node_ptr>& arr, int start_idx, int end_idx){
        if(arr.getSize() <= 0 || start_idx > end_idx){
            throw InvalidIndexInput();
        }

        int mid = (start_idx + end_idx) / 2;

        _node_ptr root = arr[mid];

        if(start_idx <= mid - 1)
            root->left = AVLTree<K,T>::_array_to_tree(arr, start_idx, mid - 1);
        if(mid + 1 <= end_idx)
            root->right = AVLTree<K,T>::_array_to_tree(arr, mid + 1, end_idx);

        return arr[mid];
    }

public:
    AVLTree() : root(nullptr), size(0){}

    AVLTree(const _node& root) : root(make_shared<_node>(root)), size(root.size){}

    AVLTree(const _node_ptr& root) : root(root), size(root->size){}

    ~AVLTree() {
        if(this->size)
            AVLTree<K,T>::_clean(this->root);
    }

    T& find(const K& key) const {
        return AVLTree<K,T>::_find(this->root, key)->data;
    }

    bool exists(const K& key) const {
        return (bool)(AVLTree<K,T>::_find(this->root, key));
    }

    void insert(const K& key, const T& data) {
        AVLTree<K,T>::_insert(this->root, key, data);
        this->size++;
    }

    void remove(const K& key) {
        if(!this->exists(key)){
            throw KeyNotExist();
        }

        AVLTree<K,T>::_remove(this->root, key);
        this->size--;
    }

    int rank(const K& key) {
        return AVLTree<K,T>::_rank(this->root, key);
    }

    const K& select(int rank) {
        if(rank <= 0 || rank > this->size){
            throw InvalidRank();
        }
        return AVLTree<K,T>::_select(this->root, rank);
    }

    int getSize() const {
        return this->size;
    }

    void print(bool keys=true, bool data=false) {
        AVLTree<K,T>::_print(this->root, keys, data, 0);
    }

    int rangeCount(K lower_bound, K upper_bound){
        return AVLTree<K,T>::_range_count(this->root, lower_bound, upper_bound);
    }

    void merge(AVLTree<K,T>& other){
        this->root = AVLTree<K,T>::_merge(this->root, other.root);

        this->size = AVLTree<K,T>::_update_size(this->root);
        AVLTree<K,T>::_update_height(this->root);
    }
};

#endif //WET2_AVLTREE_H
