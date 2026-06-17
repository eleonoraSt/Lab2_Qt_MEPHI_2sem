#ifndef NODE_H
#define NODE_H

template <class T> class Node {
    private:
        T data;
        Node<T>* prev;
        Node<T>* next;
    public:
        Node<T>(T item) {
            data = item;
            prev = nullptr;
            next = nullptr;
        }

        T GetData() const {
            return data;
        }

        Node<T>* GetPrev() const {
            return prev;
        }

        Node<T>* GetNext() const {
            return next;
        }

        void SetPrev(Node<T>* ptr) {
            prev = ptr;
        }

        void SetNext(Node<T>* ptr) {
            next = ptr;
        }
};

#endif  // NODE_H
