//
// Created by keren on 29/12/2021.
//

#ifndef UNIONFIND_H
#define UNIONFIND_H

#define DEFAULT -1

#include <exception>
#include <string>
#include <iostream>
#include <cstdio>
#include <memory>

using namespace std;

/*     EXCEPTIONS     */
class ToManySets : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "to many make sets";
    }
};
class NoSuchKey : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "this key don't exists";
    }
};
class IllegalKey : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "illegal key";
    }
};
class IllegalSize : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "illegal Size";
    }
};



template <class T>
class UnionFind {
private:
    int size;

public:
    T * arr;
    int * parents;
    int * sizes;
    int counter;

    explicit UnionFind(int size);
    ~UnionFind();
    int MakeSet(int key,const T& data);
    int find(int key);
    int Union(int p,int q);
    T& getKeyData(int key);
    int getSizeByKey(int key);

};

template <class T>
UnionFind<T>::UnionFind(int size):size(size){
    if(size <= 0){
        throw IllegalSize();
    }

    parents = new int[size];
    sizes = new int[size];
    arr = new T[size];
    counter = 0;
    for(int i = 0; i < size; i++){
        sizes[i] = 0;
        parents[i] = DEFAULT;
    }
}

template <class T>
UnionFind<T>::~UnionFind(){
    delete [] parents;
    delete [] sizes;
    delete [] arr;
}

template <class T>
int UnionFind<T>::MakeSet(int key, const T& data) {
    if (counter >= size) {
        throw ToManySets();
    }

    arr[key] = data;
    sizes[key] = 1;
    counter++;
    return key;
}

template <class T>
int UnionFind<T>:: find(int key){
    if(key < 0 || key >= size){
        throw IllegalKey();
    }
    if(sizes[key] == 0){
        throw NoSuchKey();
    }

    int group = key;
    int parent = parents[key];
    while(parent != DEFAULT){
        group = parent;
        parent = parents[parent];
    }

    int tmp = key;
    int next = parents[tmp];
    while(next != DEFAULT){
        parents[tmp] = group;
        tmp = next;
        next = parents[next];
    }

    return group;
}

template <class T>
int UnionFind<T>::Union(int p, int q){
    if(p < 0 || p >= size || q < 0 || q >= size){
        throw IllegalKey();
    }
    if(sizes[p] == 0 || sizes[q] == 0){
        throw NoSuchKey();
    }
    if(q == p){
        return p;
    }

    if(sizes[p] > sizes[q]){
        parents[q] = p;
        sizes[p] += sizes[q];
        sizes[q] = DEFAULT;//there is no more size after union, default -1
        return p;
    }

    parents[p] = q;
    sizes[q] += sizes[p];
    sizes[p] = DEFAULT;//there is no more size after union, default -1
    return q;
}

template<class T>
T& UnionFind<T>::getKeyData(int key) {
    if(key < 0 || key >= size){
        throw IllegalKey();
    }

    int head_key = this->find(key);
    return arr[head_key];
}

template<class T>
int UnionFind<T>::getSizeByKey(int key){
    int head_key = this->find(key);
    return this->sizes[head_key];
}


#endif //UNIONFIND_H
