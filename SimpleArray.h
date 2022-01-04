
#ifndef WET2_SIMPLEARRAY_H
#define WET2_SIMPLEARRAY_H

using namespace std;

class OutOfBounds : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "exceeded the bounds of the array";
    }
};

template<class T>
class SimpleArray {
private:
    T* arr;
    int size;

public:
    SimpleArray(int size): size(size) {
        this->arr = new T[size];
    }

    SimpleArray(const SimpleArray<T>& other): size(other.size) {
        this->arr = new T[other.size];
        for(int i = 0; i < other.size; i++){
            this->arr[i] = other.arr[i];
        }
    }

    ~SimpleArray() {
        delete[] this->arr;
    }

    SimpleArray<T>& operator=(const SimpleArray<T>& other) {
        if(this->arr == other.arr) {
            return *this;
        }

        this->size = other.size;

        delete[] this->arr;
        this->arr = new T[this->size];

        for(int i = 0; i < this->size; i++) {
            this->arr[i] = other.arr[i];
        }

        return *this;
    }

    T operator[](int idx) const {
        if(idx < 0 || idx >= this->size){
            throw OutOfBounds();
        }
        return this->arr[idx];
    }

    void set(int idx, const T& data) {
        if(idx < 0 || idx >= this->size){
            throw OutOfBounds();
        }
        this->arr[idx] = data;
    }

    int getSize(){
        return this->size;
    }
};

#endif //WET2_SIMPLEARRAY_H
