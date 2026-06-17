#include "..\include\errors.h"
#include "..\include\BitSequence.h"

#include <iostream>  // отладка

#define BYTE_LEN 8

BitSequence::BitSequence(const bool* bitString, int count) {
    int arraySize = count / BYTE_LEN;
    if (count % BYTE_LEN != 0) arraySize++;
    seq = new DynamicArray<char>(arraySize);
    length = 0;  // Append увеличивает length
    for (int index = 0; index < count; index++) {
        Append(bitString[index]);
        std::cout << Get(index);
    }
}

BitSequence::BitSequence() {
    seq = new DynamicArray<char>(0);
    length = 0;
}

BitSequence::BitSequence(const BitSequence &other) {
    seq = new DynamicArray<char>(*other.seq);
    length = other.length;
}

BitSequence::~BitSequence() {
    delete seq;
}

bool BitSequence::GetFirst() const {
    char mask = 0b10000000;
    std::cout << (short)(seq->Get(0)) << "\n";
    return (bool)(seq->Get(0) & mask);
}

bool BitSequence::GetLast() const {
    int bytesLength = length / BYTE_LEN;
    char mask = 0b00000001;
    if (length % BYTE_LEN != 0) {
        bytesLength++;
        mask <<= (BYTE_LEN - length % BYTE_LEN);
    }
    return (bool)(seq->Get(bytesLength - 1) & mask);
}

bool BitSequence::Get(int index) const {
    if (index < 0 || index >= length) throw INDEX_ERROR;
    char mask = 0b00000001;
    if (index % BYTE_LEN != 0) {
        mask <<= (BYTE_LEN - length % BYTE_LEN);
    }
    return (bool)(seq->Get(index / BYTE_LEN) & mask);
}

BitSequence* BitSequence::GetSubsequence(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex > length || endIndex < startIndex) throw INDEX_ERROR;
    BitSequence* subseq = new BitSequence();
    char mask = 0b10000000 >> (startIndex % BYTE_LEN);
    char byte = seq->Get(startIndex / BYTE_LEN);
    bool item;
    for (int index = startIndex; index <= endIndex; index++) {
        if (index % BYTE_LEN == 0) {
            byte = seq->Get(index / BYTE_LEN);
            mask = 0b10000000;
        }
        std::cout << "mask: " << (mask == 0b01000000) << "\n";
        item = byte & mask;
        std::cout << item;
        subseq->Append(item);
        mask >>= 1;
    }
    for (int index = 0; index <= endIndex - startIndex; index++) {
        std::cout << subseq->Get(index);
    }
    return subseq;
}

int BitSequence::GetLength() const {
    return length;
}

BitSequence* BitSequence::Append(bool item) {
    int byteIndex = length / BYTE_LEN;
    char setByte;
    char mask = 0b10000000;
    mask >>= length % BYTE_LEN;
    if (!item) mask = ~mask;  // 10000000 -> 01111111 и т.д.
    if (length % BYTE_LEN == 0) {
        seq->Resize(byteIndex + 1);
    }
    setByte = item ? seq->Get(byteIndex) | mask : seq->Get(byteIndex) & mask;
    seq->Set(byteIndex, setByte);
    length++;
    return this;
}

BitSequence* BitSequence::Prepend(bool item) {
    int lastByte = length / BYTE_LEN;
    char byte;
    if (length % BYTE_LEN == 0) {
        seq->Resize(length / BYTE_LEN + 1);
    } else {
        lastByte++;
    }
    length++;
    for (int byteIndex = lastByte; byteIndex > 0; byteIndex--) {
        byte = seq->Get(byteIndex);
        byte = byte >> 1;
        if (seq->Get(byteIndex - 1) & 0b00000001) {
            byte |= 0b10000000;
        } else {
            byte &= 0b01111111;
        }
        seq->Set(byteIndex, byte);
    }
    byte = seq->Get(0);
    byte = byte >> 1;
    byte = item ? byte | 0b10000000 : byte & 0b01111111;
    seq->Set(0, byte);
    return this;
}

BitSequence* BitSequence::InsertAt(bool item, int index) {
    if (index < 0 || index > length + 1) throw INDEX_ERROR;
    int lastByte = length / BYTE_LEN;
    int insertByte = index / BYTE_LEN;
    char byte;
    char trueMask = 0b00000001, falseMask = 0b11111110;
    if (length % BYTE_LEN == 0) {
        seq->Resize(length / BYTE_LEN + 1);
    } else {
        lastByte++;
    }
    length++;
    for (int byteIndex = lastByte; index > insertByte; index--) {
        byte = seq->Get(byteIndex);
        byte = byte >> 1;
        if (seq->Get(byteIndex - 1) & 0b00000001) {
            byte |= 0b10000000;
        } else {
            byte &= 0b01111111;
        }
        seq->Set(byteIndex, byte);
    }
    byte = seq->Get(insertByte);
    for (int bitIndex = BYTE_LEN - 1; bitIndex > index % BYTE_LEN; bitIndex++) {
        if (byte & (trueMask << 1)) {
            byte |= trueMask;
        } else {
            byte &= falseMask;
        }
        trueMask = trueMask << 1;
        falseMask = falseMask << 1;
    }
    byte = item ? byte | trueMask : byte & falseMask;
    seq->Set(insertByte, byte);
    return this;
}

BitSequence* BitSequence::Concat(Sequence<bool>* list) {
    char mask = 0b10000000;
    char byte;
    bool item;
    for (int index = 0; index < list->GetLength(); index++) {
        if (index % BYTE_LEN == 0) {
            byte = seq->Get(index / BYTE_LEN);
            mask = 0b10000000;
        }
        item = byte & mask;
        this->Append(item);
        mask >>= 1;
    }
    return this;
}

BitSequence* BitSequence::Map(bool (*func)(bool)) const {
    BitSequence* mapped = new BitSequence();
    char mask = 0b10000000;
    char byte;
    bool item;
    for (int index = 0; index < length; index++) {
        if (index % BYTE_LEN == 0) {
            byte = seq->Get(index / BYTE_LEN);
            mask = 0b10000000;
        }
        item = byte & mask;
        mapped->Append(func(item));
        mask >>= 1;
    }
    return mapped;
}

BitSequence* BitSequence::Where(bool (*func)(bool)) const {
    BitSequence* filtered = new BitSequence();
    char mask = 0b10000000;
    char byte;
    bool item;
    for (int index = 0; index < length; index++) {
        if (index % BYTE_LEN == 0) {
            byte = seq->Get(index / BYTE_LEN);
            mask = 0b10000000;
        }
        item = byte & mask;
        if (func(item)) filtered->Append(item);
        mask >>= 1;
    }
    return filtered;
}

bool BitSequence::Reduce(bool (*func)(bool, bool), bool initial) const {
    char mask = 0b10000000;
    char byte;
    bool item;
    bool result = initial;
    for (int index = 0; index < length; index++) {
        if ((index & BYTE_LEN) == 0) {
            byte = seq->Get(index / BYTE_LEN);
            mask = 0b10000000;
        }
        item = byte & mask;
        result = func(item, result);
        mask >>= 1;
    }
    return result;
}

BitSequence* BitSequence::And(const BitSequence &other) const {
    if (length != other.length) throw INDEX_ERROR;
    BitSequence* result = new BitSequence();
    char byte1, byte2, byteRes;
    char mask = 0b10000000;
    for (int index = 0; index < length; index++) {
        if ((index & BYTE_LEN) == 0) {
            byte1 = seq->Get(index / BYTE_LEN);
            byte2 = other.seq->Get(index / BYTE_LEN);
            mask = 0b10000000;
        }
        byteRes = byte1 & byte2 & mask;  // Selected bit is AND of the bytes, others =0
        result->Append((bool)byteRes);
        mask >>= 1;
    }
    return result;
}

BitSequence* BitSequence::Or(const BitSequence &other) const {
    if (length != other.length) throw INDEX_ERROR;
    BitSequence* result = new BitSequence();
    char byte1, byte2, byteRes;
    char mask = 0b10000000;
    for (int index = 0; index < length; index++) {
        if ((index & BYTE_LEN) == 0) {
            byte1 = seq->Get(index / BYTE_LEN);
            byte2 = other.seq->Get(index / BYTE_LEN);
            mask = 0b10000000;
        }
        byteRes = (byte1 | byte2) & mask;  // Selected bit is OR of the bytes, others =0
        result->Append((bool)byteRes);
        mask >>= 1;
    }
    return result;
}

BitSequence* BitSequence::Xor(const BitSequence &other) const {
    if (length != other.length) throw INDEX_ERROR;
    BitSequence* result = new BitSequence();
    char byte1, byte2, byteRes;
    char mask = 0b10000000;
    for (int index = 0; index < length; index++) {
        if ((index & BYTE_LEN) == 0) {
            byte1 = seq->Get(index / BYTE_LEN);
            byte2 = other.seq->Get(index / BYTE_LEN);
            mask = 0b10000000;
        }
        byteRes = (byte1 ^ byte2) & mask;  // Selected bit is XOR of the bytes, others =0
        result->Append((bool)byteRes);
        mask >>= 1;
    }
    return result;
}

BitSequence* BitSequence::Not() const {
    BitSequence* result = new BitSequence();
    char byte, byteRes;
    char mask = 0b10000000;
    for (int index = 0; index < length; index++) {
        if ((index & BYTE_LEN) == 0) {
            byte = seq->Get(index / BYTE_LEN);
            mask = 0b10000000;
        }
        byteRes = (byte ^ mask) & mask;  // Selected bit is NOT of the byte, others =0
        result->Append((bool)byteRes);
        mask >>= 1;
    }
    return result;
}
