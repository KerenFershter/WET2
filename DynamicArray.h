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
    std::shared_ptr<Node> prev;
    std::shared_ptr<Node> next;

    Node() = default;

    Node(int key, T& data): key(key), data(data), prev(nullptr), next(nullptr){}

    Node(const Node& other) : key(other.key), data(data.info), prev(other.prev), next(other.next){}
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

//private:
public: //TODO: private
    int size;
    std::shared_ptr<_Node> head;
    std::shared_ptr<_Node> tail;

//public:
//    friend std::ostream &operator<<(std::ostream &os, const List<T> &list);
    List() : size(0), head(nullptr), tail(nullptr){}

    void push(int key, T& data){
        std::shared_ptr<_Node> new_node (new Node<T>(key, data));
        if(tail == nullptr){
            head = new_node;
            tail = head;
        }
        else {
            new_node->prev = tail;
            tail->next = new_node;
            tail = tail->next;
        }
        size++;
    }

    void pop(){
        if(size > 1){
            tail = tail->prev;
            tail->next.reset();
            this->tail->next = nullptr;
            this->size--;
        } else{
            tail.reset();
            this->size--;
        }
    }

    void clean(){
        while (size){
            pop();
        }
    }

    ~List(){
        clean();
    }

    T& find(int key){ //TODO: assert done exist before
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

    int getLastKey(){
        if(tail){
            return tail->key;
        }
        return DEFAULT_KEY;
    }

    T& getLastData(){
        if(!tail){
            throw EmptyList();
        }
        return tail->data;
    }

    void remove(int key,bool * removed){
        std::shared_ptr<_Node> iter = this->head;
//        if(size == 1 && iter->key == key){
//            iter= nullptr;
//            head= nullptr;
//            tail= nullptr;
//            size--;
//            *removed=true;
//            return;
//        }

        while(iter != nullptr){
            if(iter->key == key){
                if(iter->prev == nullptr && iter->next == nullptr){//only one item
                    head = nullptr;
                    tail = nullptr;
                }

                if(iter->next != nullptr && iter->prev == nullptr){//removes head
                    head = iter->next;
                }

//                if(iter->prev!= nullptr){
//                    iter->prev->next=iter->next;
//                }

                if(iter->next == nullptr && iter->prev != nullptr){//tail
                    iter->prev->next = nullptr;
                }

                if(iter->next != nullptr && iter->prev != nullptr){//not head or tail
                    iter->prev->next = iter->next;
                    iter->next->prev = iter->prev;
                }

                iter = nullptr;
                size--;
                *removed = true;
                return;
            }

            iter = iter->next;
        }

        *removed = false;
    }

    int getSize(){
        return size;
    }

    bool isEmpty(){
        return (bool)this->head;
    }

//    friend std::ostream& operator<<(std::ostream& os, const List<T>& list){
//        std::shared_ptr<_Node> iter = list.head;
//        while(iter != nullptr){
//                os<< "{"<<iter->key<<","<<iter->data<<"}"<<endl;
//                iter = iter->next;
//        }
//        os<<endl;
//    }

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

public: //TODO private
    int size;
    _List *A;
//    int *B;
//    int *C;
//    T default_val;
    int counter;

//    bool is_initialized(int idx) const {
//        return (B[idx] < this->counter && B[idx] >= 0 && C[B[idx]] == idx);
//    }

//public:

    DynamicArray(int init_size=MIN_SIZE){
        size = init_size;
        counter = 0;
        A = new _List[init_size];
//        B = new int[init_size];
//        C = new int[init_size];
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

    friend std::ostream& operator<<(std::ostream& os, const DynamicArray<T>& arr){
        for(int i = 0; i < arr.size; i++){
            os << i <<":"<< endl;
            std::shared_ptr< Node<T>> iter = arr.A[i].head;
            while(iter != nullptr){
             //   os<< "{"<<iter->key<<","<<iter->data<<"}"<<endl;
                os<< "{"<<iter->key<<"}"<<endl;
                iter = iter->next;
            }
            os<<endl;
        //    os << arr.A[i] << ",\t";
        }
        return os;
    }
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
//        delete[] B;
//        delete[] C;
//        delete default_val;
}

template<class T>
void DynamicArray<T>::insert(int key, T val){
    int idx = key % this->size;
    counter++;
//    if(!is_initialized(idx)){
//        this->C[this->counter] = idx;
//        this->B[idx] = this->counter;
//        this->counter++;
//    }
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
       // float new_size=1/OM;
        resize(0.5);
    }
}

template<class T>
bool DynamicArray<T>::exists(int key){
    int idx= key % this->size;
//    if(!is_initialized(idx)){
//        return false;
//    }
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
//    delete[] this->B;
//    delete[] this->C;
//    this->B = new int[new_size];
//    this->C = new int[new_size];
    int tmpKey;
    T tmpData;
    counter = 0;
    for(int i = 0; i < tmp_size; i++) {
        while(tmp[i].getSize()){
            tmpKey = tmp[i].getLastKey();
            tmpData = tmp[i].getLastData();
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
        _List list_i = other.A[i];
        while(!list_i.isEmpty()){
            this->insert(list_i.getLastKey(), list_i.getLastData());
            list_i.pop();
        }
    }

    other.clean();
}

template<class T>
int DynamicArray<T>::getCount() {
    return this->counter;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




#endif //DYNAMICARRAY_H
