#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "errors.h"
#include "Node.h"

template <class T> class LinkedList {
    private:
        Node<T>* head;
        Node<T>* tail;
    public:
        LinkedList(const T* items, int count) {
            if (count < 0) throw NEGATIVE_SIZE;
            head = nullptr;
            tail = nullptr;
            for (int index = 0; index < count; index++) {
                this->Append(items[index]);
            }
        }

        LinkedList() {
            head = nullptr;
            tail = nullptr;
        }

        LinkedList(const LinkedList<T> &list) {
            head = nullptr;
            tail = nullptr;
            Node<T>* copyFrom = list.head;
            while (copyFrom != nullptr) {
                this->Append(copyFrom->GetData());
                copyFrom = copyFrom->GetNext();
            }
        }

        ~LinkedList() {
            Node<T>* prev;
            while (tail != nullptr) {
                prev = tail->GetPrev();
                delete tail;
                tail = prev;
            }
        }

        T GetFirst() const {
            if (head == nullptr) throw INDEX_ERROR;  // Пустой список
            return head->GetData();
        }

        T GetLast() const {
            if (tail == nullptr) throw INDEX_ERROR;  // Пустой список
            return tail->GetData();
        }

        T Get(int index) const {
            if (index < 0) throw INDEX_ERROR;
            if (head == nullptr) throw INDEX_ERROR;  // Пустой список
            Node<T>* item = head;
            for (int current = 0; current < index; current++) {
                item = item->GetNext();
                if (item == nullptr) throw INDEX_ERROR;  // Индекс больше размера массива
            }
            return item->GetData();
        }

        LinkedList<T>* GetSubList(int startIndex, int endIndex) const {
            if (startIndex < 0 || endIndex < 0 || startIndex > endIndex) throw INDEX_ERROR;
            LinkedList<T>* sublist = new LinkedList<T>();
            Node<T>* item = head;
            int index;
            for (index = 0; index < startIndex; index++) {
                if (item == nullptr) {  // Начальный индекс больше размера массива
                    delete sublist;
                    throw INDEX_ERROR;
                }
                item = item->GetNext();
            }
            for (index = startIndex; index <= endIndex; index++) {
                if (item == nullptr) {  // Конечный индекс больше размера массива
                    delete sublist;
                    throw INDEX_ERROR;
                }
                sublist->Append(item->GetData());
                item = item->GetNext();
            }
            return sublist;
        }

        int GetLength() const {
            Node<T>* item = head;
            int count = 0;
            while (item != nullptr) {
                count++;
                item = item->GetNext();
            }
            return count;
        }

        void Append(T item) {
            Node<T>* node = new Node<T>(item);
            node->SetPrev(tail);
            if (head == nullptr) {  // Пустой список
                head = node;
            } else {
                tail->SetNext(node);
            }
            tail = node;
        }

        void Prepend(T item) {
            Node<T>* node = new Node<T>(item);
            node->SetNext(head);
            if (tail == nullptr) {  // Пустой список
                tail = node;
            } else {
                head->SetPrev(node);
            }
            head = node;
        }

        void InsertAt(T item, int index) {
            if (index < 0) throw INDEX_ERROR;
            if (index == 0) {
                this->Prepend(item);
                return;
            }
            Node<T>* prev = head;
            if (prev == nullptr) throw INDEX_ERROR;
            for (int current = 0; current < index - 1; current++) {
                if (prev == nullptr) throw INDEX_ERROR;  // передан индекс больше размера списка
                prev = prev->GetNext();
            }
            Node<T>* node = new Node<T>(item);
            node->SetNext(prev->GetNext());
            node->SetPrev(prev);
            if (prev->GetNext() != nullptr) prev->GetNext()->SetPrev(node);
            prev->SetNext(node);
        }

        LinkedList<T>* Concat(LinkedList<T>* list) {
            LinkedList<T>* concatenated = new LinkedList<T>(this);
            Node<T>* otherNode = list->head;
            while (otherNode != nullptr) {
                concatenated->Append(otherNode->GetData());
            }
            return concatenated;
        }
};

#endif  // LINKED_LIST_H
