#ifndef ARRAY_SEQUENCE_H
#define ARRAY_SEQUENCE_H

#include "Sequence.h"
#include "DynamicArray.h"

template <class T> class ArraySequence: public Sequence<T> {
    private:
        DynamicArray<T>* seq;
    public:
        ArraySequence(const T* items, int count) {
            seq = new DynamicArray<T>(items, count);
        }

        ArraySequence() {
            seq = new DynamicArray<T>(0);
        }

        ArraySequence(const DynamicArray<T> &dynamicArray) {
            seq = new DynamicArray<T>(dynamicArray);
        }

        ArraySequence(const ArraySequence<T> &other) {
            seq = new DynamicArray<T>(*other.seq);
        }
/*
 * ArraySequence1 - проинициализирован
 * ArraySequence2 - проинициализирован
 * ArraySequence2 = ArraySequence1
 *
 * Правило 3: нужен конструктор по умолчанию, конструктор, собирающий объект, и конструктор, копирующий объект
 * Обязательно нужно при этом переопределить оператор присваивания
 * Правило 5: move-семантика (std::move)
 */
        ~ArraySequence() {
            delete seq;
        }

        T GetFirst() const override {
            return seq->Get(0);
        }

        T GetLast() const override {
            return seq->Get(seq->GetSize() - 1);
        }

        T Get(int index) const override {
            return seq->Get(index);
        }

        ArraySequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
            if (startIndex < 0 || endIndex > seq->GetSize() || endIndex < startIndex) throw INDEX_ERROR;
            ArraySequence<T>* subseq = new ArraySequence<T>();
            subseq->seq->Resize(endIndex - startIndex + 1);
            for (int index = startIndex; index <= endIndex; index++) {
                subseq->seq->Set(index - startIndex, seq->Get(index));
            }
            return subseq;
        }

        int GetLength() const override {
            return seq->GetSize();
        }

        ArraySequence<T>* Append(T item) override {
            seq->Resize(seq->GetSize() + 1);
            seq->Set(seq->GetSize() - 1, item);
            return this;
        }

        ArraySequence<T>* Prepend(T item) override {
            seq->Resize(seq->GetSize() + 1);
            for (int index = seq->GetSize() - 1; index > 0; index--) {
                seq->Set(index, seq->Get(index - 1));
            }
            seq->Set(0, item);
            return this;
        }

        ArraySequence<T>* InsertAt(T item, int index) override {
            seq->Resize(seq->GetSize() + 1);
            for (int current = seq->GetSize() - 1; current >= index; current--) {
                seq->Set(current, seq->Get(current - 1));
            }
            seq->Set(index, item);
            return this;
        }

        ArraySequence<T>* Concat(Sequence<T>* list) override {
            int size = seq->GetSize();
            int otherSize = list->GetLength();
            seq->Resize(seq->GetSize() + otherSize);
            for (int index = 0; index < otherSize; index++) {
                seq->Set(size + index, list->Get(index));
            }
            return this;
        }

        ArraySequence<T>* Map(T (*func)(T)) const override {
            ArraySequence<T>* mapped = new ArraySequence<T>();
            int length = seq->GetSize();
            mapped->seq->Resize(length);
            for (int index = 0; index < length; index++) {
                mapped->seq->Set(index, func(seq->Get(index)));
            }
            return mapped;
        }

        ArraySequence<T>* Where(bool (*func)(T)) const override {
            ArraySequence<T>* filtered = new ArraySequence<T>();
            int length = seq->GetSize();
            T item;
            for (int index = 0; index < length; index++) {
                item = seq->Get(index);
                if (func(item)) filtered->Append(item);
            }
            return filtered;
        }

        T Reduce(T (*func)(T, T), T initial) const override {
            T result = initial;
            int length = seq->GetSize();
            for (int index = 0; index < length; index++) {
                result = func(seq->Get(index), result);
            }
            return result;
        }
};

#endif  // ARRAY_SEQUENCE_H
