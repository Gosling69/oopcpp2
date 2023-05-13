#pragma once

#include <assert.h>

template <typename T>
class Container {

    template <typename T>
    struct Node {
        T m_value = {};
        Node * m_next = nullptr;
        Node * m_prev = nullptr;
    };

public:
    Container() { m_head = m_tail = nullptr; m_len = 0; }
    void push_tail(const T & value);
    void push_head(const T & value);
    void remove_head();
    void remove_tail();
    T & get_head() { return m_head->m_value; }
    T & get_tail() { return m_tail->m_value; }
    unsigned int get_length() { return m_len; }
    void swap(Container & other);
    void reverse();
    void clear();

    class iterator {
        Node<T> * ptr;
    public:
        iterator(Node<T> * p) { ptr = p; }
        T & operator*() { return ptr->m_value; }
        void operator=(iterator iter) { ptr = iter.ptr; }
        bool operator==(iterator iter) { return ptr == iter.ptr; }
        void operator++() { if (ptr != nullptr) ptr = ptr->m_next; }
        void operator--() { if (ptr != nullptr) ptr = ptr->m_prev; }
        bool operator!=(iterator iter) { return ptr != iter.ptr; }
    };
    iterator begin() { return iterator(m_head); }
    iterator end() { return iterator(m_tail); }

    template <typename T>
    friend std::ostream & operator<<(std::ostream & os,  Container<T> & cunt);

private:
    unsigned int  m_len;
    Node<T> * m_head, * m_tail;
};

template<typename T>
std::ostream & operator<<(std::ostream & os,  Container<T> & cont) {
    for (typename Container<T>::iterator it = cont.begin(); it != nullptr; ++it)
        os << *it << "\n";
    os << "\n";
    return os;
}

template<typename T>
void Container<T>::push_tail(const T & a) {
    Node<T> * ptr = new Node<T>;
    ptr->m_value = a;
    ptr->m_next = nullptr;
    ptr->m_prev = m_tail;
    if (m_len == 0) {
        m_head = m_tail = ptr;
    }
    else {
        m_tail->m_next = ptr;
        m_tail = ptr;
    }
    ++m_len;
}

template<typename T>
void Container<T>::push_head(const T & a) {
    Node<T> * ptr = new Node<T>;
    ptr->m_value = a;
    ptr->m_next = m_head;
    ptr->m_prev = nullptr;
    if (m_len == 0) {
        m_head = m_tail = ptr;
    }
    else {
        m_head->m_prev = ptr;
        m_head = ptr;
    }
    ++m_len;
}

template<typename T>
void Container<T>::remove_head() {
    if (m_head == nullptr) {
        throw std::runtime_error("Empty list");
    }
    Node<T> * temp = m_head;
    if (temp->m_next == nullptr) {
        m_head = nullptr;
    }
    else {
        m_head = m_head->m_next;
    }
    T res = temp->m_value;
    delete temp;
    --m_len;
    assert(m_len >= 0);
}

template<typename T>
void Container<T>::remove_tail() {
    if (m_tail == nullptr) {
        throw std::runtime_error("Empty list");
    }
    Node<T> * temp = m_tail;
    m_tail = m_tail -> m_prev;
    if (m_tail) {
        m_tail->m_next = nullptr;
    }
    else {
        m_head = nullptr;
    }
    T res = temp->m_value;
    delete temp;
    --m_len;
    assert(m_len >= 0);

}

template<typename T>
void Container<T>::clear() {
    if (m_head == nullptr) return;
    Node<T> * temp = m_head;
    Node<T> * next = nullptr;
    while (temp != nullptr) {
        delete temp;
        temp = next;
    }
    m_head = m_tail = nullptr;
    m_len = 0;
    assert(m_len >= 0);
}

template<typename T>
void Container<T>::reverse() {
    Node<T> * copy_head = m_head;
    Node<T> * ptr = m_tail;
    if (ptr == nullptr) {
        return;
    }
    ptr->m_next = ptr->m_prev;
    ptr->m_prev = nullptr;
    while (ptr->m_next != nullptr) {
        Node<T> * ptr2 = ptr->m_next;
        ptr2->m_next = ptr2->m_prev;
        ptr2->m_prev = ptr;
        ptr = ptr2;
    }
    m_head = m_tail;
    m_tail = copy_head;
}

template<typename T>
void Container<T>::swap(Container & other) {
    Node<T> * new_head = other.m_head;
    Node<T> * new_tail = other.m_tail;
    unsigned int new_len = other.get_length();
    other.m_head = m_head;
    other.m_tail = m_tail;
    other.m_len = m_len;
    m_head = new_head;
    m_tail = new_tail;
    m_len = new_len;
}