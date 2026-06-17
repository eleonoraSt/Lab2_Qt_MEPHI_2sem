#ifndef IMMUTABLE_LIST_SEQUENCE_H
#define IMMUTABLE_LIST_SEQUENCE_H

#include "Sequence.h"
#include "LinkedList.h"

template <class T> class ImmutableListSequence: public Sequence<T> {
private:
    LinkedList<T>* seq;
public:
    ImmutableListSequence(const T* items, int count) {
        seq = new LinkedList<T>(items, count);
    }

    ImmutableListSequence() {
        seq = new LinkedList<T>();
    }

    ImmutableListSequence(const LinkedList<T> &linkedList) {
        seq = new LinkedList<T>(linkedList);
    }

    ImmutableListSequence(const ImmutableListSequence<T> &other) {
        seq = new LinkedList<T>(*other.seq);
    }

    ~ImmutableListSequence() {
        delete seq;
    }

    T GetFirst() const override {
        return seq->GetFirst();
    }

    T GetLast() const override {
        return seq->GetLast();
    }

    T Get(int index) const override {
        return seq->Get(index);
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        ImmutableListSequence<T>* subseq = new ImmutableListSequence<T>();
        subseq->seq = seq->GetSubList(startIndex, endIndex);
        return subseq;
    }

    int GetLength() const override {
        return seq->GetLength();
    }

    Sequence<T>* Append(T item) override {
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>(*this);
        result->seq->Append(item);
        return result;
    }

    Sequence<T>* Prepend(T item) override {
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>(*this);
        result->seq->Prepend(item);
        return result;
    }

    Sequence<T>* InsertAt(T item, int index) override {
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>(*this);
        result->seq->InsertAt(item, index);
        return result;
    }

    Sequence<T>* Concat(Sequence<T>* list) override {
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>(*this);
        int length = list->GetLength();
        for (int index = 0; index < length; index++) {
            result->seq->Append(list->Get(index));
        }
        return result;
    }

    Sequence<T>* Map(T (*func)(T)) const override {
        ImmutableListSequence<T>* mapped = new ImmutableListSequence<T>();
        int length = seq->GetLength();
        for (int index = 0; index < length; index++) {
            mapped->Append(func(seq->Get(index)));
        }
        return mapped;
    }

    Sequence<T>* Where(bool (*func)(T)) const override {
        ImmutableListSequence<T>* filtered = new ImmutableListSequence<T>();
        int length = seq->GetLength();
        T item;
        for (int index = 0; index < length; index++) {
            item = seq->Get(index);
            if (func(item)) filtered->Append(item);
        }
        return filtered;
    }

    T Reduce(T (*func)(T, T), T initial) const override {
        T result = initial;
        int length = seq->GetLength();
        for (int index = 0; index < length; index++) {
            result = func(seq->Get(index), result);
        }
        return result;
    }
};

#endif // IMMUTABLE_LIST_SEQUENCE_H
