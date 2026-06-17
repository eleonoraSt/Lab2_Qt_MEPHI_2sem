#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "errors.h"

template <class T> class DynamicArray {
    private:
        T* data;
        int size;
    public:
        DynamicArray(const T* items, int count) {
            if (count < 0) throw NEGATIVE_SIZE;
            size = count;
            if (count == 0) {
                data = nullptr;
                return;
            }
            data = new T[count];
            for (int index = 0; index < count; index++) {
                data[index] = items[index];
            }
        }

        DynamicArray(int count) {
            if (count < 0) throw NEGATIVE_SIZE;
            size = count;
            if (count == 0) {
                data = nullptr;
                return;
            }
            data = new T[count];
        }

        DynamicArray(const DynamicArray<T> &dynamicArray) {
            size = dynamicArray.GetSize();
            data = new T[size];
            for (int index = 0; index < size; index++) {
                data[index] = dynamicArray.Get(index);
            }
        }

        ~DynamicArray() {
            delete[] data;
        }
        
        T Get(int index) const {
            if (index < 0 || index >= size) throw INDEX_ERROR;
            return data[index];
        }

        int GetSize() const {
            return size;
        }

        void Set(int index, T value) {
            if (index < 0 || index >= size) throw INDEX_ERROR;
            data[index] = value;
        }

        void Resize(int newSize) {
            if (newSize < 0) throw NEGATIVE_SIZE;
            T* newData = newSize == 0 ? nullptr : new T[newSize];
            int copyNum = size <= newSize ? size : newSize;
            for (int index = 0; index < copyNum; index++) {
                newData[index] = data[index];
            }
            delete[] data;
            data = newData;
            size = newSize;
        }
};

#endif  // DYNAMIC_ARRAY_H
