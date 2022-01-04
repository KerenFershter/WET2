//
// Created by keren on 03/01/2022.
//

#ifndef AVLTREE_H
#define AVLTREE_H

#include "memory"
#include "Utils.h"


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
        static const bool _exists(const _node_ptr& root, const K& key){
            if(AVLTree<K,T>::_find(root, key)== nullptr){
                return false;
            }
            return true;
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

    public:
        AVLTree() : root(nullptr), size(0){}

        ~AVLTree() {
            if(this->size)
                AVLTree<K,T>::_clean(this->root);
        }

        T& find(const K& key) const {
            return AVLTree<K,T>::_find(this->root, key)->data;
        }

        void insert(const K& key, const T& data) {
            AVLTree<K,T>::_insert(this->root, key, data);
            this->size++;
        }

        void remove(const K& key) {
            if(!this->find(key)){
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

        void print(bool keys=true, bool data=false)const {
            AVLTree<K,T>::_print(this->root, keys, data, 0);
        }

        int rangeCount(K lower_bound, K upper_bound){
            return AVLTree<K,T>::_range_count(this->root, lower_bound, upper_bound);
        }

        bool exists(const K& key){
            return AVLTree<K,T>::_exists(this->root, key);
        }

    };


#endif //AVLTREE_H
