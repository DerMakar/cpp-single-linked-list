#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <iostream>
#include <stdexcept>
 
using namespace std;
 
template <typename Type>
class SingleLinkedList {
struct Node {
    Node() = default;
    Node(const Type& val, Node* next)
        : value(val)
        , next_node(next) {
    }
    Type value;
    Node* next_node = nullptr;
};
 
template <typename ValueType>
class BasicIterator {
    friend class SingleLinkedList;
    explicit BasicIterator(Node* node) {
        node_ = node;
    }
 
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Type;
    using difference_type = std::ptrdiff_t;
    using pointer = ValueType*;
    using reference = ValueType&;
 
    BasicIterator() = default;
 
    BasicIterator(const BasicIterator<Type>& other) noexcept {
        node_ = other.node_;
    }
 
    BasicIterator& operator=(const BasicIterator& rhs) = default;
 
    template <class OtherType>
        [[nodiscard]] bool operator==(const BasicIterator<OtherType>& rhs) const noexcept {
            return (node_ == rhs.node_);
        }
       
    template <class OtherType>
        [[nodiscard]] bool operator!=(const BasicIterator<OtherType>& rhs) const noexcept {
            return !(*this == rhs);
        }
      
    BasicIterator& operator++() {
        if(node_){ 
	        node_ = node_->next_node;
            return *this;
        }else{
            assert(false);
        }
    }
 
    BasicIterator operator++(int)  {
	    if(node_){            
		    auto i = *this;
            ++(*this);
            return i;
	    }else{
            throw std::logic_error("log"s);
        }
    }
 
    [[nodiscard]] reference operator*() const  {
	    if(node_){            
	        return node_->value;
        }else{
	        throw std::logic_error("log"s);
        }
    }
 
    [[nodiscard]] pointer operator->() const noexcept {
        if(node_){
          return &node_->value;
        }else{
            assert(false);
        }
    }
 
private:
    Node* node_ = nullptr;
};
 
public:
    SingleLinkedList<Type>  () {}
 
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }
 
    [[nodiscard]] bool IsEmpty() const noexcept {
        if (size_ == 0){
            return true;
        }
        return false;
    }
 
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
 
    void Clear() noexcept {
        while (head_.next_node != nullptr){
                delete std::exchange(head_.next_node, head_.next_node->next_node);
        }
        size_ = 0;
    }
 
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
 
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;
 
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }
 
    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }
 
    [[nodiscard]] ConstIterator begin() const noexcept {
        return cbegin();
    }
 
    [[nodiscard]] ConstIterator end() const noexcept {
        return cend();
    }
 
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }
 
    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }
    
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{static_cast<Node*>(&head_)};
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{const_cast<Node*>(&head_)};
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator{const_cast<Node*>(&head_)};
    }
    
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        if(!pos.node_){
            assert(false);
        }
        pos.node_->next_node = new Node(value, pos.node_->next_node);
        ++size_;
        return Iterator(pos.node_->next_node);
    }

    void PopFront() noexcept {
        if (IsEmpty()) assert(false);
        Node* it =  head_.next_node;
        head_.next_node = it -> next_node;
        delete it;
        size_ -= 1;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        if (IsEmpty()) assert(false);
        auto it_ = pos.node_ -> next_node;
        pos.node_->next_node = it_->next_node;
        delete it_;
        size_ -= 1;
        return Iterator(pos.node_->next_node);
    }
   
    SingleLinkedList(std::initializer_list<Type> values) {
        if(values.size() != 0){
            auto iter = cbefore_begin();
            for (auto it = values.begin(); it != values.end(); ++it){
                InsertAfter(iter, *it);
                ++iter;
            }
        }
    }
 
    SingleLinkedList(const SingleLinkedList& other) {
        if (!other.IsEmpty()){
            auto iter = cbefore_begin();           
            for (auto it = other.begin(); it != other.end(); ++it){
                    InsertAfter(iter, *it);
                    ++iter;
                }
            }
    }
 
    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if(this != &rhs){
            SingleLinkedList tmp (rhs);
            swap(tmp);
        }
        return *this;
    }
 
    void swap(SingleLinkedList& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(head_.next_node, other.head_.next_node);
    }
 
    ~ SingleLinkedList(){
        Clear();
    }   
 
private:
    Node head_;
    size_t size_ = 0;
};
 
template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}
 
template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return lhs.GetSize() == rhs.GetSize() && equal(lhs.begin(),lhs.end(),rhs.cbegin(),rhs.end());
}
 
template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}
 
template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return lexicographical_compare(lhs.cbegin(),lhs.cend(),rhs.cbegin(),rhs.cend());
}
 
template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}
 
template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs <= rhs);
}
 
template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs > lhs);
}
