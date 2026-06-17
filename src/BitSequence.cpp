#include "..\include\errors.h"
#include "..\include\BitSequence.h"

#include <iostream>  // отладка

#define BYTE_LEN 8

BitSequence::BitSequence(const bool* bits, int count) {
    seq = new DynamicArray<bool>(count);
    for (int index = 0; index < count; index++) {
        seq->Set(index, bits[index]);
    }
}

BitSequence::BitSequence() {
    seq = new DynamicArray<bool>(0);
}

BitSequence::BitSequence(const BitSequence &other) {
    seq = new DynamicArray<bool>(*other.seq);
}

BitSequence::~BitSequence() {
    delete seq;
}

bool BitSequence::GetFirst() const {
    return seq->Get(0);
}

bool BitSequence::GetLast() const {
    return seq->Get(seq->GetSize() - 1);
}

bool BitSequence::Get(int index) const {
    return seq->Get(index);
}

BitSequence* BitSequence::GetSubsequence(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex > seq->GetSize() || endIndex < startIndex) throw INDEX_ERROR;
    BitSequence* subseq = new BitSequence();
    subseq->seq->Resize(endIndex - startIndex + 1);
    for (int index = startIndex; index <= endIndex; index++) {
        subseq->seq->Set(index - startIndex, seq->Get(index));
    }
    return subseq;
}

int BitSequence::GetLength() const {
    return seq->GetSize();
}

BitSequence* BitSequence::Append(bool item) {
    int oldSize = seq->GetSize();
    seq->Resize(oldSize + 1);
    seq->Set(oldSize, item);
    return this;
}

BitSequence* BitSequence::Prepend(bool item) {
    int oldSize = seq->GetSize();
    seq->Resize(oldSize + 1);
    for (int index = oldSize; index < 0; index--) {
        seq->Set(index, seq->Get(index - 1));
    }
    seq->Set(0, item);
    return this;
}

BitSequence* BitSequence::InsertAt(bool item, int index) {
    int oldSize = seq->GetSize();
    seq->Resize(oldSize + 1);
    for (int indexSeq = oldSize; indexSeq < index; indexSeq--) {
        seq->Set(indexSeq, seq->Get(indexSeq - 1));
    }
    seq->Set(index, item);
    return this;
}

BitSequence* BitSequence::Concat(Sequence<bool>* list) {
    int otherSize = list->GetLength();
    seq->Resize(seq->GetSize() + otherSize);
    for (int index = 0; index < otherSize; index++) {
        seq->Set(index + seq->GetSize(), list->Get(index));
    }
    return this;
}

BitSequence* BitSequence::Map(bool (*func)(bool)) const {
    BitSequence* mapped = new BitSequence();
    mapped->seq->Resize(seq->GetSize());
    for (int index = 0; index < seq->GetSize(); index++) {
        mapped->seq->Set(index, func(seq->Get(index)));
    }
    return mapped;
}

BitSequence* BitSequence::Where(bool (*func)(bool)) const {
    BitSequence* filtered = new BitSequence();
    bool item;
    for (int index = 0; index < seq->GetSize(); index++) {
        item = seq->Get(index);
        if (func(item)) {
            filtered->Append(item);
        }
    }
    return filtered;
}

bool BitSequence::Reduce(bool (*func)(bool, bool), bool initial) const {
    bool result = initial;
    for (int index = 0; index < seq->GetSize(); index++) {
        result = func(seq->Get(index), result);
    }
    return result;
}

BitSequence* BitSequence::And(const BitSequence &other) const {
    if (seq->GetSize() != other.seq->GetSize()) throw INDEX_ERROR;
    BitSequence* result = new BitSequence;
    result->seq->Resize(seq->GetSize());
    for (int index = 0; index < seq->GetSize(); index++) {
        result->seq->Set(index, seq->Get(index) && other.Get(index));
    }
    return result;
}

BitSequence* BitSequence::Or(const BitSequence &other) const {
    if (seq->GetSize() != other.seq->GetSize()) throw INDEX_ERROR;
    BitSequence* result = new BitSequence;
    result->seq->Resize(seq->GetSize());
    for (int index = 0; index < seq->GetSize(); index++) {
        result->seq->Set(index, seq->Get(index) || other.Get(index));
    }
    return result;
}

BitSequence* BitSequence::Xor(const BitSequence &other) const {
    if (seq->GetSize() != other.seq->GetSize()) throw INDEX_ERROR;
    BitSequence* result = new BitSequence;
    result->seq->Resize(seq->GetSize());
    for (int index = 0; index < seq->GetSize(); index++) {
        result->seq->Set(index, (seq->Get(index) || other.Get(index)) && \
            !(seq->Get(index) && other.Get(index)));
    }
    return result;
}

BitSequence* BitSequence::Not() const {
    BitSequence* result = new BitSequence;
    result->seq->Resize(seq->GetSize());
    for (int index = 0; index < seq->GetSize(); index++) {
        result->seq->Set(index, !(seq->Get(index)));
    }
    return result;
}
