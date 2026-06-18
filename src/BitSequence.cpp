#include "..\include\errors.h"
#include "..\include\BitSequence.h"

#include <iostream>  // отладка

#define BYTE_LEN 8

BitSequence::BitSequence(const bool* bitString, int count) {
    seq = new DynamicArray<unsigned char>(0);
    length = 0;  // Append увеличивает length
    for (int index = 0; index < count; index++) {
        Append(bitString[index]);
    }
}

BitSequence::BitSequence() {
    seq = new DynamicArray<unsigned char>(0);
    length = 0;
}

BitSequence::BitSequence(const BitSequence &other) {
    seq = new DynamicArray<unsigned char>(*other.seq);
    length = other.length;
}

BitSequence::~BitSequence() {
    delete seq;
}

bool BitSequence::GetFirst() const {
    unsigned char mask = 128;
    std::cout << (short)(seq->Get(0)) << "\n";
    return (bool)(seq->Get(0) & mask);
}

bool BitSequence::GetLast() const {
    unsigned char mask = 1;
    for (int i = 1; i < BYTE_LEN - (length - 1) % BYTE_LEN; i++) {
        mask *= 2;
    }
    return (bool)(seq->Get((length - 1) / BYTE_LEN) & mask);
}

bool BitSequence::Get(int index) const {
    if (index < 0 || index >= length) throw INDEX_ERROR;
    unsigned char mask = 1;
    for (int i = 1; i < BYTE_LEN - index % BYTE_LEN; i++) {
        mask *= 2;
    }
    return (bool)(seq->Get(index / BYTE_LEN) & mask);
}

BitSequence* BitSequence::GetSubsequence(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex > length || endIndex < startIndex) throw INDEX_ERROR;
    BitSequence* subseq = new BitSequence();
    for (int index = startIndex; index <= endIndex; index++) {
        subseq->Append(Get(index));
    }
    return subseq;
}

int BitSequence::GetLength() const {
    return length;
}

BitSequence* BitSequence::Append(bool item) {
    unsigned char byte = 0;
    if (length % BYTE_LEN == 0) {
        seq->Resize(length / BYTE_LEN + 1);
    } else {
        byte = seq->Get((length - 1) / BYTE_LEN);
    }
    unsigned char add = 128;
    if (item) {  // по умолчанию 0
        for (int i = 0; i < length % BYTE_LEN; i++) {
            add /= 2;
        }
        byte += add;
    }
    length++;
    seq->Set(length / BYTE_LEN, byte);
    return this;
}

BitSequence* BitSequence::Prepend(bool item) {
    if (length % BYTE_LEN == 0) {
        seq->Resize(length / BYTE_LEN + 1);
        seq->Set(length / BYTE_LEN, 0);
    }
    length++;
    unsigned char byte;
    for (int i = (length - 1) / BYTE_LEN; i > 0; i--) {
        byte = seq->Get(i) / 2;
        if (seq->Get(i - 1) % 2 == 1) {
            byte += 128;
        }
        seq->Set(i, byte);
    }
    byte = seq->Get(0) / 2;
    if (item) {
        byte += 128;
    }
    seq->Set(0, byte);
    return this;
}

BitSequence* BitSequence::InsertAt(bool item, int index) {
    if (index < 0 || index > length) throw INDEX_ERROR;
    std::cout << "the error is not here\n";
    if (length % BYTE_LEN == 0) {
        seq->Resize(length / BYTE_LEN + 1);
        seq->Set(length / BYTE_LEN, 0);
    }
    BitSequence* buf = new BitSequence();
    for (int i = index; i < length; i++) {
        buf->Append(Get(i));
    }
    seq->Resize(index / BYTE_LEN + 1);
    unsigned char byte = seq->Get(index / BYTE_LEN);
    int twoDegree = 1;
    for (int i = 0; i < BYTE_LEN - index % BYTE_LEN; i++) {
        twoDegree *= 2;
    }
    byte = (unsigned char)(byte - byte % twoDegree) + (unsigned char)(item * (twoDegree / 2));
    seq->Set(index / BYTE_LEN, byte);
    length = index + 1;
    for (int i = 0; i < buf->length; i++) {
        Append(buf->Get(i));
    }
    delete buf;
    return this;
}

BitSequence* BitSequence::Concat(Sequence<bool>* list) {
    for (int index = 0; index < list->GetLength(); index++) {
        Append(list->Get(index));
    }
    return this;
}

BitSequence* BitSequence::Map(bool (*func)(bool)) const {
    BitSequence* mapped = new BitSequence();
    for (int index = 0; index < length; index++) {
        mapped->Append(func(Get(index)));
    }
    return mapped;
}

BitSequence* BitSequence::Where(bool (*func)(bool)) const {
    BitSequence* filtered = new BitSequence();
    bool item;
    for (int index = 0; index < length; index++) {
        item = Get(index);
        if (func(item)) {
            filtered->Append(item);
        }
    }
    return filtered;
}

bool BitSequence::Reduce(bool (*func)(bool, bool), bool initial) const {
    bool result = initial;
    for (int index = 0; index < length; index++) {
        result = func(Get(index), result);
    }
    return result;
}

BitSequence* BitSequence::And(const BitSequence &other) const {
    if (length != other.length) throw INDEX_ERROR;
    BitSequence* result = new BitSequence();
    result->seq->Resize((length - 1) / BYTE_LEN + 1);
    for (int index = 0; index <= (length - 1) / BYTE_LEN; index++) {
        result->seq->Set(index, seq->Get(index) & other.seq->Get(index));
    }
    return result;
}

BitSequence* BitSequence::Or(const BitSequence &other) const {
    if (length != other.length) throw INDEX_ERROR;
    BitSequence* result = new BitSequence();
    result->seq->Resize((length - 1) / BYTE_LEN + 1);
    for (int index = 0; index <= (length - 1) / BYTE_LEN; index++) {
        result->seq->Set(index, seq->Get(index) | other.seq->Get(index));
    }
    return result;
}

BitSequence* BitSequence::Xor(const BitSequence &other) const {
    if (length != other.length) throw INDEX_ERROR;
    BitSequence* result = new BitSequence();
    result->seq->Resize((length - 1) / BYTE_LEN + 1);
    for (int index = 0; index <= (length - 1) / BYTE_LEN; index++) {
        result->seq->Set(index, seq->Get(index) ^ other.seq->Get(index));
    }
    return result;
}

BitSequence* BitSequence::Not() const {
    BitSequence* result = new BitSequence();
    result->seq->Resize((length - 1) / BYTE_LEN + 1);
    for (int index = 0; index <= (length - 1) / BYTE_LEN; index++) {
        result->seq->Set(index, ~seq->Get(index));
    }
    return result;
}
