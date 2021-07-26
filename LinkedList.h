//
// Created by sunday on 6/28/21.
//

#ifndef OSPCBQUEUE_LINKEDLIST_H
#define OSPCBQUEUE_LINKEDLIST_H

#pragma once

#include <iostream>

template<typename T>
class LinkedList {
    struct Node {
        T m_data;
        Node* m_next = nullptr;
        Node* m_last = nullptr;
        explicit Node(T data):m_data(data) {}
        ~Node() {
            m_next = nullptr;
            m_last = nullptr;
        }
    };
    Node* m_head = nullptr;
    Node* m_tail = nullptr;
    int m_size = 0;
    int index = 0;
public:
    LinkedList() = default;
    LinkedList(const LinkedList<T>& list);
    ~LinkedList();
    LinkedList<T>& operator=(const LinkedList<T>& list);

    //Capacity
    [[nodiscard]] bool empty() const;
    [[nodiscard]] int size() const;

    //Element access
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    T operator[](size_t n) const;

    //Modifiers
    void push_front(const T& data);
    void push_back(const T& data);
    void pop_back();
    T pop_front();
    bool delete_element(T& data);

    //other
    bool contains(const T& data);
    void clear();
    T* getElement(const T& data);
    T* getNext();
    void incrementIndex();
    void decrementIndex();
    void resetIndex();

    template<typename Y>
    friend std::ostream& operator<<(std::ostream& out, const LinkedList<Y>& list);
};

template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& list) {
    Node* curr = list.m_head;
    while(curr != nullptr) {
        push_back(curr->m_data);
        curr = curr->m_next;
    }
    m_size = list.m_size;
}

template<typename T>
LinkedList<T>::~LinkedList() {
    if(m_head == nullptr) return;

    while(m_head->m_next != nullptr) {
        m_head = m_head->m_next;
        delete m_head->m_last;
        m_head->m_last = nullptr;
    }
    delete m_head;
    m_head = nullptr;
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& list) {
    if(this == &list) return *this;
    if(m_size > 0) clear();

    Node* curr = list.m_head;
    while(curr != nullptr) {
        push_back(curr->m_data);
        curr = curr->m_next;
    }

    m_size = list.m_size;
    index = 0;

    return *this;
}

template<typename T>
bool LinkedList<T>::empty() const {
    return !m_size;
}


template<typename T>
int LinkedList<T>::size() const {
    return m_size;
}

template<typename T>
T& LinkedList<T>::front() {
    return m_head->m_data;
}

template<typename T>
const T& LinkedList<T>::front() const {
    return m_head->m_data;
}

template<typename T>
T& LinkedList<T>::back() {
    return m_tail->m_data;
}

template<typename T>
const T& LinkedList<T>::back() const {
    return m_tail->m_data;
}

template<typename T>
T LinkedList<T>::operator[](size_t n) const  // Make function `const` since it will not modify the internal data
{
    Node* current = m_head;

    while (current != nullptr && n-- > 0)
    {
        current = current->m_next;
    }
    return (current->m_data);
}

template<typename T>
void LinkedList<T>::push_front(const T& data) {
    Node* node = new Node(data);
    if(m_head != nullptr) {
        node->m_next = m_head;
        m_head->m_last = node;
    }
    if(m_tail == nullptr) m_tail = node;
    m_head = node;
    m_size++;
}

template<typename T>
void LinkedList<T>::push_back(const T& data){
    Node* node = new Node(data);
    if(m_tail != nullptr) {
        node->m_last = m_tail;
        m_tail->m_next = node;
    }
    if(m_head == nullptr) m_head = node;
    m_tail = node;
    m_size++;
}

template<typename T>
void LinkedList<T>::pop_back() {
    if(m_tail == nullptr) return;

    Node* temp = m_tail;
    m_tail = m_tail->m_last;
    if(m_tail != nullptr) m_tail->m_next = nullptr;
    delete temp;
    m_size--;
    if(m_size == 0) m_head = nullptr;
}

template<typename T>
T LinkedList<T>::pop_front() {

    Node* temp = m_head;
    m_head = m_head->m_next;
    if(m_head != nullptr) m_head->m_last = nullptr;
    m_size--;
    if(m_size == 0) m_head = nullptr;
    T returnvalue = temp->m_data;
    delete temp;
    return returnvalue;
}

template<typename T>
bool LinkedList<T>::delete_element(T &data) {
    Node* curr = m_head;
    while(curr != nullptr) {
        if(curr->m_data == data){
            if(curr==m_head){
                if(curr->m_next){
                    curr->m_next->m_last= nullptr;
                    m_head=curr->m_next;
                }else{
                    m_head=nullptr;
                    m_tail=nullptr;
                }
            }
            else if(curr==m_tail){
                curr->m_last->m_next=nullptr;
                m_tail=curr->m_last;
            }
            else{
                curr->m_last->m_next=curr->m_next;
                curr->m_next->m_last=curr->m_last;
            }
            delete curr;
            m_size--;
            return true;
        }
        curr=curr->m_next;
    }
    return false;
}

template<typename T>
bool LinkedList<T>::contains(const T& data) {
    Node* curr = m_head;
    while(curr != nullptr) {
        if(curr->m_data == data) return true;
        curr = curr->m_next;
    }
    return false;
}

template<typename T>
void LinkedList<T>::clear() {
    if(m_head == nullptr) return;

    while(m_head->m_next != nullptr) {
        m_head = m_head->m_next;
        delete m_head->m_last;
        m_head->m_last = nullptr;
    }
    delete m_head;
    m_head = nullptr;
    m_tail = nullptr;

    m_size = 0;
    index = 0;
}

template<typename T>
T* LinkedList<T>::getElement(const T& data) {
    Node* curr = m_head;
    while(curr != nullptr) {
        if(curr->m_data == data) return &curr->m_data;
        curr = curr->m_next;
    }
    return nullptr;
}

template<typename T>
T* LinkedList<T>::getNext() {
    Node* curr = m_head;
    if(index + 1 > m_size) {
        return nullptr;
    }
    for(int x = 0; x < index; x++) {
        curr = curr->m_next;
    }

    index++;
    return &curr->m_data;
}

template<typename T>
void LinkedList<T>::incrementIndex() {
    index++;
}

template<typename T>
void LinkedList<T>::decrementIndex() {
    index--;
}

template<typename T>
void LinkedList<T>::resetIndex() {
    index = 0;
}

template<typename Y>
std::ostream& operator<<(std::ostream& out, const LinkedList<Y>& list) {
    auto* curr = list.m_head;
    while(curr!= nullptr) {
        std::cout << curr->m_data;
        curr = curr->m_next;
    }

    return out;
}
#endif //OSPCBQUEUE_LINKEDLIST_H
