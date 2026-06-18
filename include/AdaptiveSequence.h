#ifndef ADAPTIVE_SEQUENCE_H
#define ADAPTIVE_SEQUENCE_H

#include "ArraySequence.h"
#include "ListSequence.h"

#define RECENT_ACT_COUNT 10
#define GET_ACTION 1
#define SET_ACTION -1
#define DEFAULT_ACTION 0

enum AdaptiveSeqMode {
    ARRAY = 0,
    LIST
};

template <class T> class AdaptiveSequence: public Sequence<T> {
    private:
        Sequence<T>* seq;
        mutable AdaptiveSeqMode mode;
        mutable short* recentActions;
        mutable short recentActionCount;
        mutable short sumActions;

        void SwitchToArray() {
            int size = seq->GetLength();
            DynamicArray<T>* array = new DynamicArray<T>(0);
            array->Resize(size);
            for (int index = 0; index < size; index++) {
                array->Set(index, seq->Get(index));
            }
            ArraySequence<T>* newSeq = new ArraySequence<T>(*array);
            delete seq;
            seq = newSeq;
        }

        void SwitchToList() {
            int size = seq->GetLength();
            LinkedList<T>* list = new LinkedList<T>();
            for (int index = 0; index < size; index++) {
                list->Append(seq->Get(index));
            }
            ListSequence<T>* newSeq = new ListSequence<T>(*list);
            delete seq;
            seq = newSeq;
        }
    public:
        AdaptiveSequence(const T* items, int count, AdaptiveSeqMode givenMode) {
            mode = givenMode;
            switch (mode) {
            case ARRAY:
                seq = new ArraySequence<T>(items, count);
                break;
            case LIST:
                seq = new ListSequence<T>(items, count);
                break;
            default:
                throw ADAPTIVE_SEQ_MODE_ERROR;
            }
            recentActions = new short[RECENT_ACT_COUNT];
            for (short index = 0; index < RECENT_ACT_COUNT; index++) {
                recentActions[index] = DEFAULT_ACTION;
            }
            recentActionCount = 0;
            sumActions = 0;
        }

        AdaptiveSequence(AdaptiveSeqMode givenMode) {
            mode = givenMode;
            switch (mode) {
            case ARRAY:
                seq = new ArraySequence<T>();
                break;
            case LIST:
                seq = new ListSequence<T>();
                break;
            default:
                throw ADAPTIVE_SEQ_MODE_ERROR;
            }
            recentActions = new short[RECENT_ACT_COUNT];
            for (short index = 0; index < RECENT_ACT_COUNT; index++) {
                recentActions[index] = DEFAULT_ACTION;
            }
            recentActionCount = 0;
            sumActions = 0;
        }

        AdaptiveSequence(const AdaptiveSequence<T> &other) {
            mode = other.mode;
            switch (other.mode) {
            case ARRAY:
                seq = new ArraySequence<T>(*other.seq);
                break;
            case LIST:
                seq = new ListSequence<T>(*other.seq);
                break;
            default:  // не должно достигаться
                throw ADAPTIVE_SEQ_MODE_ERROR;
            }
            recentActions = new short[RECENT_ACT_COUNT];
            for (short index = 0; index < RECENT_ACT_COUNT; index++) {
                recentActions[index] = DEFAULT_ACTION;
            }
            recentActionCount = 0;
            sumActions = 0;
        }

        ~AdaptiveSequence() {
            delete seq;
            delete recentActions;
        }

        T GetFirst() const override {
            return seq->GetFirst();
        }

        T GetLast() const override {
            return seq->GetLast();
        }

        T Get(int index) const override {
            sumActions = sumActions - recentActions[recentActionCount] + GET_ACTION;
            recentActions[recentActionCount] = GET_ACTION;
            recentActionCount = (recentActionCount + 1) % RECENT_ACT_COUNT;
            return seq->Get(index);
        }

        AdaptiveSequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
            sumActions = sumActions - recentActions[recentActionCount] + GET_ACTION;
            recentActions[recentActionCount] = GET_ACTION;
            recentActionCount = (recentActionCount + 1) % RECENT_ACT_COUNT;
            AdaptiveSequence<T>* result = new AdaptiveSequence<T>(mode);
            result->seq = seq->GetSubsequence(startIndex, endIndex);
            return result;
        }

        int GetLength() const override {
            return seq->GetLength();
        }

        AdaptiveSequence<T>* Append(T item) override {
            sumActions = sumActions - recentActions[recentActionCount] + SET_ACTION;
            recentActions[recentActionCount] = SET_ACTION;
            recentActionCount = (recentActionCount + 1) % RECENT_ACT_COUNT;
            seq->Append(item);
            return this;
        }

        AdaptiveSequence<T>* Prepend(T item) override {
            sumActions = sumActions - recentActions[recentActionCount] + SET_ACTION;
            recentActions[recentActionCount] = SET_ACTION;
            recentActionCount = (recentActionCount + 1) % RECENT_ACT_COUNT;
            seq->Prepend(item);
            return this;
        }

        AdaptiveSequence<T>* InsertAt(T item, int index) override {
            sumActions = sumActions - recentActions[recentActionCount] + SET_ACTION;
            recentActions[recentActionCount] = SET_ACTION;
            recentActionCount = (recentActionCount + 1) % RECENT_ACT_COUNT;
            seq->InsertAt(item, index);
            return this;
        }

        AdaptiveSequence<T>* Concat(Sequence<T>* list) override {
            sumActions = sumActions - recentActions[recentActionCount] + SET_ACTION;
            recentActions[recentActionCount] = SET_ACTION;
            recentActionCount = (recentActionCount + 1) % RECENT_ACT_COUNT;
            if (sumActions < 0 && mode == ARRAY) {
                SwitchToList();
            }
            seq->Concat(list);
            return this;
        }

        AdaptiveSequence<T>* Map(T (*func)(T)) const override {
            AdaptiveSequence<T>* result = new AdaptiveSequence<T>(mode);
            result->seq = seq->Map(func);
            return result;
        }

        AdaptiveSequence<T>* Where(bool (*func)(T)) const override {
            AdaptiveSequence<T>* result = new AdaptiveSequence<T>(mode);
            result->seq = seq->Where(func);
            return result;
        }

        T Reduce(T (*func)(T, T), T initial) const override {
            return seq->Reduce(func, initial);
        }

        AdaptiveSeqMode GetMode() const {
            return mode;
        }

        void SwitchMode() {
            if (mode == ARRAY) {
                SwitchToList();
            } else {
                SwitchToArray();
            }
        }

        void AdaptMode() {
            if (sumActions > 0 && mode == LIST) SwitchToArray();
            if (sumActions < 0 && mode == ARRAY) SwitchToList();
        }
};

#endif
