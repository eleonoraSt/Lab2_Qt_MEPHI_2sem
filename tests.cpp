#include <iostream>

#include "include/ListSequence.h"
#include "include/ArraySequence.h"
#include "include/ImmutableListSequence.h"
#include "include/ImmutableArraySequence.h"
#include "include/AdaptiveSequence.h"
#include "include/BitSequence.h"

template <class T> void testGetFirst(short* testNum, Sequence<T>* seq, T expected) {
    try {
        T result = seq->GetFirst();
        if (result == expected) {
            std::cout << "Test " << *testNum << " passed\n";
        } else {
            std::cout << "Test " << *testNum << " failed: expected " << expected << ", got " \
                      << result << " in GetFirst\n";
        }
    } catch (ErrorCode error) {
        std::cout << "Test " << *testNum << " failed: error in GetFirst\n";
    }
    *testNum += 1;
}

template <class T> void testGetFirstIndexError(short* testNum, Sequence<T>* seq) {
    try {
        seq->GetFirst();
        std::cout << "Test " << *testNum << " failed: error not raised in GetFirst\n";
    } catch (ErrorCode error) {
        std::cout << "Test " << *testNum << " passed\n";
    }
    *testNum += 1;
}

template <class T> void testGetLast(short* testNum, Sequence<T>* seq, T expected) {
    try {
        T result = seq->GetLast();
        if (result == expected) {
            std::cout << "Test " << *testNum << " passed\n";
        } else {
            std::cout << "Test " << *testNum << " failed: expected " << expected << ", got " << result << \
                " in GetLast\n";
        }
    } catch (ErrorCode error) {
        std::cout << "Test " << *testNum << " failed: error in GetLast\n";
    }
    *testNum += 1;
}

template <class T> void testGetLastIndexError(short* testNum, Sequence<T>* seq) {
    try {
        seq->GetLast();
        std::cout << "Test " << *testNum << " failed: error not raised in GetLast\n";
    } catch (ErrorCode error) {
        std::cout << "Test " << *testNum << " passed\n";
    }
    *testNum += 1;
}

template <class T> void testGet(short *testNum, Sequence<T>* seq, T expected, int index) {
    try {
        T result = seq->Get(index);
        if (result == expected) {
            std::cout << "Test " << *testNum << " passed\n";
        } else {
            std::cout << "Test " << *testNum << " failed: expected " << expected << " at index " << \
                index << ", got " << result << " in Get\n";
        }
    } catch (ErrorCode error) {
        std::cout << "Test " << *testNum << " failed: error with size " << seq->GetLength() << \
            ", index " << index << " in Get\n";
    }
    *testNum += 1;
}

template <class T> void testGetIndexError(short* testNum, Sequence<T>* seq, int index) {
    try {
        seq->Get(index);
        std::cout << "Test " << *testNum << " failed: size " << seq->GetLength() << ", index " << \
            index << ", error not raised in Get\n";
    } catch (ErrorCode error) {
        std::cout << "Test " << *testNum << " passed\n";
    }
    *testNum += 1;
}

template <class T> void testGetSubseq(short* testNum, Sequence<T>* seq, Sequence<T>* expected, int start, \
                   int end) {
    try {
        Sequence<T>* subseq = seq->GetSubsequence(start, end);
        int size = subseq->GetLength(), expectedSize = expected->GetLength();
        int index = 0;
        bool equal = size == expectedSize;
        while (index < size && equal) {
            equal = expected->Get(index) == subseq->Get(index);
            index++;
        }
        if (equal) {
            std::cout << "Test " << *testNum << " passed\n";
        } else {
            std::cout << "Test " << *testNum << " failed: expected \n";
            for (int index = 0; index < expectedSize; index++) {
                std::cout << expected->Get(index) << " ";
            }
            std::cout << "\ngot\n";
            for (int index = 0; index < size; index++) {
                std::cout << subseq->Get(index) << " ";
            }
            std::cout << " in GetSubsequence\n";
        }
        delete subseq;
    } catch (ErrorCode error) {
        std::cout << "Test " << *testNum << " failed: error with size " << seq->GetLength() << \
            ", subseq from " << start << " to " << end << " in GetSubsequence\n";
    }
    *testNum += 1;
}

template <class T> void testGetSubseqIndexError(short* testNum, Sequence<T>* seq, int start, int end) {
    try  {
        Sequence<T>* subseq = seq->GetSubsequence(start, end);
        std::cout << "Test " << *testNum << " failed: size " << seq->GetLength() << ", subseq from " << \
            start << " to " << end << ", error not raised in GetSubsequence\n";
        delete subseq;
    } catch (ErrorCode error) {
        std::cout << "Test " << *testNum << " passed\n";
    }
    *testNum += 1;
}

template <class T> void testGetLength(short* testNum, Sequence<T>* seq, int expected) {
    int size = seq->GetLength();
    if (size == expected) {
        std::cout << "Test " << *testNum << " passed\n";
    } else {
        std::cout << "Test " << *testNum << " failed: expected " << expected << ", got " << size << \
            " in GetLength\n";
    }
    *testNum += 1;
}

template <class T> void testAppend(short* testNum, Sequence<T>* seq, T toAdd) {
    seq->Append(toAdd);
    T last = seq->GetLast();
    if (last == toAdd) {
        std::cout << "Test " << *testNum << " passed\n";
    } else {
        std::cout << "Test " << *testNum << " failed: expected last " << toAdd << ", got " << last << \
            " after Append\n";
    }
    *testNum += 1;
}

template <class T> void testPrepend(short* testNum, Sequence<T>* seq, T toAdd) {
    seq->Prepend(toAdd);
    T first = seq->GetFirst();
    if (first == toAdd) {
        std::cout << "Test " << *testNum << " passed\n";
    } else {
        std::cout << "Test " << *testNum << " failed: expected last " << toAdd << ", got " << first << \
            " after Prepend\n";
    }
    *testNum += 1;
}

template <class T> void testInsertAt(short* testNum, Sequence<T>* seq, T toAdd, int index) {
    try {
        seq->InsertAt(toAdd, index);
        T inPlace = seq->Get(index);
        if (inPlace == toAdd) {
            std::cout << "Test " << *testNum << " passed\n";
        } else {
            std::cout << "Test " << *testNum << " failed: expected " << toAdd << "at index " << index \
                      << ", got " << inPlace << " after InsertAt\n";
        }
    } catch (ErrorCode error) {
        std::cout << "Test " << *testNum << " failed: error in InsertAt\n";
    }
    *testNum += 1;
}

template <class T> void testInsertAtIndexError(short* testNum, Sequence<T>* seq, T toAdd, int index) {
    try {
        seq->InsertAt(toAdd, index);
        std::cout << "Test " << *testNum << " failed: inserting " << toAdd << " at index " << index << \
            ", error not raised in InsertAt\n";
    } catch (ErrorCode error) {
        std::cout << "Test " << *testNum << " passed\n";
    }
    *testNum += 1;
}

template <class T> void testConcat(short* testNum, Sequence<T>* seq, Sequence<T>* toConnect) {
    int oldSize1 = seq->GetLength(), oldSize2 = toConnect->GetLength();
    seq->Concat(toConnect);
    bool valid = seq->GetLength() == oldSize1 + oldSize2;
    if (!valid) {
        std::cout << "Test " << *testNum << " failed: concatenated size expected " << \
            oldSize1 + oldSize2 << ", got " << seq->GetLength() << "\n";
        return;
    }
    int index = 0;
    for (; index < oldSize2 && valid; index++) {
        valid = seq->Get(oldSize1 + index) == toConnect->Get(index);
    }
    if (valid) {
        std::cout << "Test " << *testNum << " passed\n";
    } else {
        std::cout << "Test " << *testNum << " failed: element number " << index << " expected " << \
            toConnect->Get(index) << ", got " << seq->Get(index + oldSize1);
    }
    *testNum += 1;
}

template <class T> void testSwitchMode(short* testNum, AdaptiveSequence<T>* seq) {
    AdaptiveSequence<T>* switchedSeq = new AdaptiveSequence<T>(*seq);
    switchedSeq->SwitchMode();
    if (seq->GetMode() == switchedSeq->GetMode()) {
        std::cout << "Test " << *testNum << " failed: both modes are " << \
            seq->GetMode() == ARRAY ? "array\n" : "list\n";
        delete switchedSeq;
        return;
    }
    int size = seq->GetLength(), switchedSize = switchedSeq->GetLength();
    if (size != switchedSeq) {
        std::cout << "Test " << *testNum << " failed: expected size " << size << ", got " \
                  << switchedSize << "\n";
        delete switchedSeq;
        return;
    }
    int index = 0;
    bool valid = true;
    for (; index < size && valid; index++) {
        valid = seq->Get(index) == switchedSeq->Get(index);
    }
    if (valid) {
        std::cout << "Test " << *testNum << " passed\n";
    } else {
        std::cout << "Test " << *testNum << " failed: element number " << index << " expected " << \
            seq->Get(index) << ", got " << switchedSeq->Get(index) << "\n";
    }
    delete switchedSeq;
    *testNum += 1;
}

void testAnd(short* testNum, BitSequence* seq1, BitSequence* seq2) {
    BitSequence* result;
    try {
        result = seq1->And(*seq2);
    } catch (ErrorCode error) {
        std::cout << "Test " << *testNum << " failed: sizes " << seq1->GetLength() << " and " << \
            seq2->GetLength() << ", error in And\n";
        return;
    }

    int size = result->GetLength(), expectedSize = seq1->GetLength();
    if (size != expectedSize) {
        std::cout << "Test " << *testNum << " failed: expected size " << expectedSize << ", got " << \
            size << " in And\n";
        delete result;
        return;
    }
    int index = 0;
    bool valid = true;
    bool val1, val2;
    for (; index < size && valid; index++) {
        valid = (seq1->Get(index) && seq2->Get(index)) == result->Get(index);
    }
    if (valid) {
        std::cout << "Test " << *testNum << " passed\n";
    } else {
        std::cout << "Test " << *testNum << " failed: element number " << index << " expected " << \
            std::to_string(seq1->Get(index) && seq2->Get(index)) << ", got " << result->Get(index) << \
            " in And\n";
    }
    delete result;
    *testNum += 1;
}

void testAndIndexError(short* testNum, BitSequence* seq1, BitSequence* seq2) {
    BitSequence* result;
    try {
        result = seq1->And(*seq2);
        std::cout << "Test " << *testNum << " failed: sizes " << seq1->GetLength() << " and " << \
            seq2->GetLength() << ", error not raised in And\n";
        delete result;
    } catch (ErrorCode error) {
        std::cout << "Test " << *testNum << " passed\n";
    }
    *testNum += 1;
}

void testOr(short* testNum, BitSequence* seq1, BitSequence* seq2) {
    BitSequence* result;
    try {
        result = seq1->Or(*seq2);
    } catch (ErrorCode error) {
        std::cout << "Test " << *testNum << " failed: sizes " << seq1->GetLength() << " and " << \
            seq2->GetLength() << ", error in Or\n";
        return;
    }

    int size = result->GetLength(), expectedSize = seq1->GetLength();
    if (size != expectedSize) {
        std::cout << "Test " << *testNum << " failed: expected size " << expectedSize << ", got " << \
            size << " in Or\n";
        delete result;
        return;
    }
    int index = 0;
    bool valid = true;
    for (; index < size && valid; index++) {
        valid = (seq1->Get(index) || seq2->Get(index)) == result->Get(index);
    }
    if (valid) {
        std::cout << "Test " << *testNum << " passed\n";
    } else {
        std::cout << "Test " << *testNum << " failed: element number " << index << " expected " << \
            std::to_string(seq1->Get(index) || seq2->Get(index)) << ", got " << result->Get(index) << \
            " in Or\n";
    }
    delete result;
    *testNum += 1;
}

void testOrIndexError(short* testNum, BitSequence* seq1, BitSequence* seq2) {
    BitSequence* result;
    try {
        result = seq1->Or(*seq2);
        std::cout << "Test " << *testNum << " failed: sizes " << seq1->GetLength() << " and " << \
            seq2->GetLength() << ", error not raised in Or\n";
        delete result;
    } catch (ErrorCode error) {
        std::cout << "Test " << *testNum << " passed\n";
    }
    *testNum += 1;
}

void testXor(short* testNum, BitSequence* seq1, BitSequence* seq2) {
    BitSequence* result;
    try {
        result = seq1->Xor(*seq2);
    } catch (ErrorCode error) {
        std::cout << "Test " << *testNum << " failed: sizes " << seq1->GetLength() << " and " << \
            seq2->GetLength() << ", error in Xor\n";
        return;
    }

    int size = result->GetLength(), expectedSize = seq1->GetLength();
    if (size != expectedSize) {
        std::cout << "Test " << *testNum << " failed: expected size " << expectedSize << ", got " << \
            size << " in Xor\n";
        delete result;
        return;
    }
    int index = 0;
    bool valid = true;
    for (; index < size && valid; index++) {
        valid = (!(seq1->Get(index) && seq2->Get(index)) && (seq1->Get(index) || seq2->Get(index))) \
                                                               == result->Get(index);
    }
    if (valid) {
        std::cout << "Test " << *testNum << " passed\n";
    } else {
        std::cout << "Test " << *testNum << " failed: element number " << index << " expected " << \
        std::to_string(!(seq1->Get(index) && seq2->Get(index)) && (seq1->Get(index) || seq2->Get(index))) \
        << ", got " << result->Get(index) << " in Xor\n";
    }
    delete result;
    *testNum += 1;
}

void testXorIndexError(short* testNum, BitSequence* seq1, BitSequence* seq2) {
    BitSequence* result;
    try {
        result = seq1->Xor(*seq2);
        std::cout << "Test " << *testNum << " failed: sizes " << seq1->GetLength() << " and " << \
            seq2->GetLength() << ", error not raised in Xor\n";
        delete result;
    } catch (ErrorCode error) {
        std::cout << "Test " << *testNum << " passed\n";
    }
    *testNum += 1;
}

void testNot(short* testNum, BitSequence* seq) {
    BitSequence* result = seq->Not();
    int size = seq->GetLength(), resultSize = result->GetLength();
    if (size != resultSize) {
        std::cout << "Test " << *testNum << " failed: expected size " << size << ", got" << resultSize \
                  << " in Not\n";
        delete result;
        return;
    }
    int index = 0;
    bool valid = true;
    for (; index < size && valid; index++) {
        valid = !(seq->Get(index) == result->Get(index));
    }
    if (valid) {
        std::cout << "Test " << *testNum << " passed\n";
    } else {
        std::cout << "Test " << *testNum << " failed: element number " << index << " expected " << \
            !(seq->Get(index)) << ", got " << result->Get(index) << " in Not\n";
    }
    *testNum += 1;
}

void runAllTests() {
    short testNum = 1;
    short* testNumAddress = &testNum;
    int baseArray[] = {1, 2, 3};
    bool baseBoolArray[] = {true, false, true, false};
    int baseArraySize = 3;

    ListSequence<int>* intList = new ListSequence<int>(baseArray, baseArraySize);
    ArraySequence<int>* intArray = new ArraySequence<int>(baseArray, baseArraySize);
    ListSequence<int>* emptyList = new ListSequence<int>();
    ArraySequence<int>* emptyArray = new ArraySequence<int>();
    AdaptiveSequence<int>* intAdaptive = new AdaptiveSequence<int>(baseArray, baseArraySize, ARRAY);
    BitSequence* bitSeq = new BitSequence(baseBoolArray, baseArraySize + 1);

    testGetFirst<int>(testNumAddress, intList, 1);
    testGetFirst<int>(testNumAddress, intArray, 1);
    testGetFirstIndexError<int>(testNumAddress, emptyList);
    testGetFirstIndexError<int>(testNumAddress, emptyArray);
    testGetFirst<int>(testNumAddress, intAdaptive, 1);
    testGetFirst<bool>(testNumAddress, bitSeq, true);  //6

    testGetLast<int>(testNumAddress, intList, 3);
    testGetLast<int>(testNumAddress, intArray, 3);
    testGetLastIndexError<int>(testNumAddress, emptyList);
    testGetLastIndexError<int>(testNumAddress, emptyArray);
    testGetLast<int>(testNumAddress, intAdaptive, 3);
    testGetLast<bool>(testNumAddress, bitSeq, false);  //12

    testGet<int>(testNumAddress, intList, 2, 1);
    testGet<int>(testNumAddress, intArray, 2, 1);
    testGet<int>(testNumAddress, intAdaptive, 2, 1);
    testGet<bool>(testNumAddress, bitSeq, false, 1);  //16

    testGetIndexError(testNumAddress, emptyList, 3);
    testGetIndexError(testNumAddress, emptyArray, 3);
    testGetIndexError(testNumAddress, intAdaptive, 3);
    testGetIndexError(testNumAddress, bitSeq, 4);  //20

    int subseqArray[] = {2, 3};
    bool subseqBoolArray[] = {false, true};
    ArraySequence<int>* expectedSubseq = new ArraySequence<int>(subseqArray, 2);  // type of seq doesn't matter
    BitSequence* expectedBitSubseq = new BitSequence(subseqBoolArray, 2);

    testGetSubseq(testNumAddress, intList, expectedSubseq, 1, 2);
    testGetSubseq(testNumAddress, intArray, expectedSubseq, 1, 2);
    testGetSubseq(testNumAddress, intAdaptive, expectedSubseq, 1, 2);
    testGetSubseq(testNumAddress, bitSeq, expectedBitSubseq, 1, 2);  // 24

    delete expectedSubseq;
    delete expectedBitSubseq;

    testGetSubseqIndexError(testNumAddress, intList, -1, 2);
    testGetSubseqIndexError(testNumAddress, intList, 2, 1);
    testGetSubseqIndexError(testNumAddress, intArray, -1, 2);
    testGetSubseqIndexError(testNumAddress, intArray, 2, 1);
    testGetSubseqIndexError(testNumAddress, intAdaptive, -1, 2);
    testGetSubseqIndexError(testNumAddress, intAdaptive, 2, 1);
    testGetSubseqIndexError(testNumAddress, bitSeq, -1, 2);
    testGetSubseqIndexError(testNumAddress, bitSeq, 2, 1);  //32

    testGetLength(testNumAddress, intList, 3);
    testGetLength(testNumAddress, intArray, 3);
    testGetLength(testNumAddress, emptyList, 0);
    testGetLength(testNumAddress, emptyArray, 0);
    testGetLength(testNumAddress, intAdaptive, 3);
    testGetLength(testNumAddress, bitSeq, 4);  //38

    testAppend(testNumAddress, intList, 5);
    testAppend(testNumAddress, intArray, 5);
    testAppend(testNumAddress, intAdaptive, 5);
    testAppend(testNumAddress, bitSeq, false);  //42

    testPrepend(testNumAddress, intList, 0);
    testPrepend(testNumAddress, intArray, 0);
    testPrepend(testNumAddress, intAdaptive, 0);
    testPrepend(testNumAddress, bitSeq, false);  // 46

    testInsertAt(testNumAddress, intList, 4, 4);
    testInsertAt(testNumAddress, intArray, 4, 4);
    testInsertAt(testNumAddress, intAdaptive, 4, 4);
    testInsertAt(testNumAddress, bitSeq, true, 5);  //50

    testInsertAtIndexError(testNumAddress, emptyList, 1, 1);
    testInsertAtIndexError(testNumAddress, emptyArray, 1, 1);  //52

    testConcat(testNumAddress, intList, intAdaptive);
    testConcat(testNumAddress, intArray, intAdaptive);  //56

    // If everything's right, bitSeq is now 0101010
    bool bitMaskArray[] = {false, false, false, true, true, true, true};
    bool bitMaskArrayIndexError[] = {true};
    BitSequence* bitMask = new BitSequence(bitMaskArray, 7);
    BitSequence* bitMaskIndexError = new BitSequence(bitMaskArrayIndexError, 1);

    testAnd(testNumAddress, bitSeq, bitMask);
    testOr(testNumAddress, bitSeq, bitMask);
    testXor(testNumAddress, bitSeq, bitMask);
    testNot(testNumAddress, bitSeq);

    testAndIndexError(testNumAddress, bitSeq, bitMaskIndexError);
    testOrIndexError(testNumAddress, bitSeq, bitMaskIndexError);
    testXorIndexError(testNumAddress, bitSeq, bitMaskIndexError);

    delete bitMask;
    delete bitMaskIndexError;

    delete intList;
    delete intArray;
    delete emptyList;
    delete emptyArray;
    delete intAdaptive;
    delete bitSeq;
}