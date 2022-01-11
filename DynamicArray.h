//
// Created by keren on 31/12/2021.
//

#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include <memory>
#include <exception>
#include <iostream>

#define MIN_SIZE 10
#define DEFAULT_KEY -1
#define OM 2

using namespace std;
class NotExist : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "not exist!!!! need to do exist before";
    }
};

///////////////////////////////////////////
// class Node
///////////////////////////////////////////
template<class T>
struct Node {
    int key;
    T data;
    std::shared_ptr<Node> next;

    Node() = default;

   Node(int key, T& data): key(key), data(data), next(nullptr){}
   Node(const Node& other) : key(other.key), data(data.info), next(other.next){}
};


///////////////////////////////////////////
// class List
///////////////////////////////////////////

class EmptyList : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "cant get data of element in empty list";
    }
};

template<class T>
class List {
    typedef Node<T> _Node;

private:
    int size;
    std::shared_ptr<_Node> head;

public:
    List() : size(0), head(nullptr){}

    void push(int key, T& data){
        if(head == nullptr){
            head = make_shared<Node<T>>(key, data);
        }
        else {
            std::shared_ptr<_Node> tmp = head;
            head = make_shared<Node<T>>(key, data);
            head->next= tmp;
        }
        size++;
    }

    void pop(){
        if(head != nullptr) {
            std::shared_ptr<_Node> tmp = head;
            head = head->next;
            tmp.reset();
            this->size--;
        }
    }

    void clean(){
        while (head!= nullptr){
            pop();
        }
    }

    ~List(){
        clean();
    }

    T& find(int key){
        std::shared_ptr<_Node> iter = head;
        while(iter != nullptr){
            if(iter->key == key){
                return iter->data;
            }
            iter = iter->next;
        }
        throw NotExist();
    }

    bool exists(int key){
        std::shared_ptr<_Node> iter = head;
        while(iter != nullptr){
            if(iter->key == key){
                return true;
            }
            iter = iter->next;
        }
        return false;
    }

    int getFirstKey(){
        if(head){
            return head->key;
        }
        return DEFAULT_KEY;
    }

    T& getFirstData(){
        if(!head){
            throw EmptyList();
        }
        return head->data;
    }

    void remove(int key,bool * removed){
        std::shared_ptr<_Node> iter = this->head;
        if(iter== nullptr){//empty list
            *removed = false;
            return;
        }
        if( iter->next == nullptr){//only one item
            if(iter->key == key){//removes head
                head = nullptr;
                iter = nullptr;
                size--;
                *removed = true;
            }
            *removed = false;
            return;
        }
        std::shared_ptr<_Node> iter_prev=iter;
        iter = iter->next;
        while(iter != nullptr){
            if(iter->key == key){
                if( iter->next == nullptr){//removes last
                    iter_prev->next = nullptr;
                }

                if(iter->next != nullptr){//not head or last
                    iter_prev->next = iter->next;
                }

                iter = nullptr;
                iter_prev= nullptr;
                size--;
                *removed = true;
                return;
            }
            iter_prev=iter;
            iter = iter->next;

        }

        *removed = false;
    }

    int getSize(){
        return size;
    }

    bool isEmpty(){
        if(head== nullptr){
            return true;
        }
        return false;
    }

    template<typename P>
    void apply(P pred, void* arg){
            std::shared_ptr<_Node> iter = this->head;
            while(iter){
                pred(iter->data, arg);
                iter = iter->next;
            }
        }
    };




///////////////////////////////////////////
// class dynamic and hash
///////////////////////////////////////////


template<class T>
class DynamicArray {
    typedef List<T> _List;

private:
    int size;
    _List *A;
    int counter;

public:

    DynamicArray(int init_size=MIN_SIZE){
        size = init_size;
        counter = 0;
        A = new _List[init_size];
    }

    DynamicArray(DynamicArray &&other) noexcept : DynamicArray()
    {
        std::swap(*this, other);
    };

    ~DynamicArray();
    void clean();
    void insert(int key, T val);
    void remove(int key);
    bool exists(int key);
    T& find(int key);
    void resize(float factor = 2.);
    void merge(DynamicArray<T>& other);
    int getCount();

    template<typename P>
    void apply(P pred, void* arg){
        for(int i = 0; i < this->size; i++){
            _List list_i = this->A[i];
            if(!list_i.isEmpty()){
                list_i.apply(pred, arg);
            }
        }
    }

//    friend std::ostream& operator<<(std::ostream& os, const DynamicArray<T>& arr){
//        for(int i = 0; i < arr.size; i++){
//            os << i << ":" << endl;
//            std::shared_ptr< Node<T>> iter = arr.A[i].head;
//            while(iter != nullptr){
//                os << "{" << iter->key << "}" << endl;
//                iter = iter->next;
//            }
//            os << endl;
//        }
//        return os;
//    }
};


template<class T>
DynamicArray<T>::~DynamicArray(){
    this->clean();
}

template<class T>
void DynamicArray<T>::clean(){
    for(int i = 0; i < size; i++){
        A[i].clean();
    }

    delete [] A;
}

template<class T>
void DynamicArray<T>::insert(int key, T val){
    int idx = key % this->size;
    counter++;
    A[idx].push(key, val);
    if(counter == size * OM){
        this->resize(OM);
    }
}

template<class T>
void DynamicArray<T>::remove(int key) {
    bool removed = false;
    A[key % size].remove(key, &removed);
    if(removed){
        counter--;
    }
    if (counter <= size / OM){
        resize(1.0 / (float)OM);
    }
}

template<class T>
bool DynamicArray<T>::exists(int key){
    int idx = key % this->size;
    return A[idx].exists(key);
}

template<class T>
T& DynamicArray<T>::find(int key){
    return A[key % size].find(key);
}

template<class T>
void DynamicArray<T>::resize(float factor){
    int new_size = this->size * factor;
    if(new_size < MIN_SIZE){
        new_size = MIN_SIZE;
    }

    _List* tmp = A;
    int tmp_size = size;
    A = new _List[new_size];
    this->size = new_size;

    int tmpKey;
    T tmpData;
    counter = 0;
    for(int i = 0; i < tmp_size; i++) {
        while(tmp[i].getSize()){
            tmpKey = tmp[i].getFirstKey();
            tmpData = tmp[i].getFirstData();
            this->insert(tmpKey, tmpData);
            tmp[i].pop();
        }
    }

    delete[] tmp;
}

template<class T>
void DynamicArray<T>::merge(DynamicArray<T>& other){
    int other_list_size = other.size;
    for(int i = 0; i < other_list_size; i++){
        _List * list_i = (&other.A[i]);
        while(!list_i->isEmpty()){
            this->insert(list_i->getFirstKey(), list_i->getFirstData());
            list_i->pop();
        }
    }
}

template<class T>
int DynamicArray<T>::getCount() {
    return this->counter;
}


#endif //DYNAMICARRAY_H
