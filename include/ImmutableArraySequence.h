#ifndef IMMUTABLE_ARRAY_SEQUENCE_H
#define IMMUTABLE_ARRAY_SEQUENCE_H

#include "Sequence.h"
#include "DynamicArray.h"

template <class T> class ImmutableArraySequence: public Sequence<T> {
    private:
        DynamicArray<T>* seq;
    public:
        ImmutableArraySequence(const T* items, int count) {
            seq = new DynamicArray<T>(items, count);
        }

        ImmutableArraySequence() {
            seq = new DynamicArray<T>(0);
        }

        ImmutableArraySequence(const DynamicArray<T> &dynamicArray) {
            seq = new DynamicArray<T>(dynamicArray);
        }

        ImmutableArraySequence(const ImmutableArraySequence<T> &other) {
            seq = new DynamicArray<T>(*other.seq);
        }

        ~ImmutableArraySequence() {
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

        ImmutableArraySequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
            ImmutableArraySequence<T>* subseq = new ImmutableArraySequence<T>();
            subseq->seq->Resize(endIndex - startIndex + 1);
            for (int index = startIndex; index < endIndex; index++) {
                subseq->seq->Set(index - startIndex, seq->Get(index));
            }
            return subseq;
        }

        int GetLength() const override {
            return seq->GetSize();
        }

        ImmutableArraySequence<T>* Append(T item) override {
            ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>();
            result->seq->Resize(seq->GetSize() + 1);
            result->seq->Set(seq->GetSize() - 1, item);
            return result;
        }

        ImmutableArraySequence<T>* Prepend(T item) override {
            ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>();
            result->seq->Resize(seq->GetSize() + 1);
            for (int index = seq->GetSize() - 1; index >= 0; index--) {
                result->seq->Set(index, seq->Get(index - 1));
            }
            result->seq->Set(0, item);
            return result;
        }

        ImmutableArraySequence<T>* InsertAt(T item, int index) override {
            ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>();
            result->seq->Resize(seq->GetSize() + 1);
            for (int current = seq->GetSize() - 1; current >= index; current--) {
                result->seq->Set(current, seq->Get(current - 1));
            }
            result->seq->Set(index, item);
            return this;
        }

        ImmutableArraySequence<T>* Concat(Sequence<T>* list) override {
            ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>();
            int size = seq->GetSize();
            int otherSize = list->GetLength();
            result->seq->Resize(seq->GetSize() + otherSize);
            for (int index = 0; index < otherSize; index++) {
                result->seq->Set(size + index, list->Get(index));
            }
            return this;
        }

        ImmutableArraySequence<T>* Map(T (*func)(T)) const override {
            ImmutableArraySequence<T>* mapped = new ImmutableArraySequence<T>();
            int length = seq->GetSize();
            mapped->seq->Resize(length);
            for (int index = 0; index < length; index++) {
                mapped->seq->Set(index, func(seq->Get(index)));
            }
            return mapped;
        }

        ImmutableArraySequence<T>* Where(bool (*func)(T)) const override {
            ImmutableArraySequence<T>* filtered = new ImmutableArraySequence<T>();
            int length = seq->GetSize();
            int filteredIndex = 0;
            T item;
            for (int index = 0; index < length; index++) {
                item = seq->Get(index);
                if (func(item)) {
                    filtered->seq->Set(filteredIndex, item);
                    filteredIndex++;
                }
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

#endif  // IMMUTABLE_ARRAY_SEQUENCE_H
