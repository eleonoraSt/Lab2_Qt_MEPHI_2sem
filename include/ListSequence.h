#ifndef LIST_SEQUENCE_H
#define LIST_SEQUENCE_H

#include "Sequence.h"
#include "LinkedList.h"

template <class T> class ListSequence: public Sequence<T> {
private:
    LinkedList<T>* seq;
public:
    ListSequence(const T* items, int count) {
        seq = new LinkedList<T>(items, count);
    }

    ListSequence() {
        seq = new LinkedList<T>();
    }

    ListSequence(const LinkedList<T> &linkedList) {
        seq = new LinkedList<T>(linkedList);
    }

    ListSequence(const ListSequence<T> &other) {
        seq = new LinkedList<T>(*other.seq);
    }

    ~ListSequence() {
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
        ListSequence<T>* subseq = new ListSequence<T>();
        subseq->seq = seq->GetSubList(startIndex, endIndex);
        return subseq;
    }

    int GetLength() const override {
        return seq->GetLength();
    }

    Sequence<T>* Append(T item) override {
        seq->Append(item);
        return this;
    }

    Sequence<T>* Prepend(T item) override {
        seq->Prepend(item);
        return this;
    }

    Sequence<T>* InsertAt(T item, int index) override {
        seq->InsertAt(item, index);
        return this;
    }

    Sequence<T>* Concat(Sequence<T>* list) override {
        int length = list->GetLength();
        for (int index = 0; index < length; index++) {
            seq->Append(list->Get(index));
        }
        return this;
    }

    Sequence<T>* Map(T (*func)(T)) const override {
        ListSequence<T>* mapped = new ListSequence<T>();
        int length = seq->GetLength();
        for (int index = 0; index < length; index++) {
            mapped->Append(func(seq->Get(index)));
        }
        return mapped;
    }

    Sequence<T>* Where(bool (*func)(T)) const override {
        ListSequence<T>* filtered = new ListSequence<T>();
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

#endif // LIST_SEQUENCE_H
