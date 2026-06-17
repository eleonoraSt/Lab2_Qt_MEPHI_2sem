#ifndef BIT_SEQUENCE
#define BIT_SEQUENCE

#include "Sequence.h"
#include "DynamicArray.h"

class BitSequence: public Sequence<bool> {
private:
    DynamicArray<char>* seq;  // Один char - 8 бит
    int length;  // Поскольку выделено памяти всегда на количество, кратное 8
public:
    BitSequence(const bool* bitString, int count);
    BitSequence();
    BitSequence(const BitSequence &other);
    ~BitSequence();

    bool GetFirst() const override;
    bool GetLast() const override;
    bool Get(int index) const override;
    BitSequence* GetSubsequence(int startIndex, int endIndex) const override;
    int GetLength() const override;

    BitSequence* Append(bool item) override;
    BitSequence* Prepend(bool item) override;
    BitSequence* InsertAt(bool item, int index) override;
    BitSequence* Concat(Sequence<bool>* list) override;

    BitSequence* Map(bool (*func)(bool)) const override;
    BitSequence* Where(bool (*func)(bool)) const override;
    bool Reduce(bool (*func)(bool, bool), bool initial) const override;

    BitSequence* And(const BitSequence &other) const;
    BitSequence* Or(const BitSequence &other) const;
    BitSequence* Xor(const BitSequence &other) const;
    BitSequence* Not() const;
};

#endif  // BIT_SEQUENCE
