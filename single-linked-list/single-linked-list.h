#pragma once
#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <algorithm>
 
template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value{};
        Node* next_node = nullptr;
    };
 
    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;
        // Конвертирующий конструктор итератора из указателя на узел списка
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
 
 
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }
 
 
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_ != rhs.node_;
        }
 

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }
 
     
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ != rhs.node_;
        }
 
   
        BasicIterator& operator++() noexcept {
            assert(node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }
        
        
        BasicIterator operator++(int) noexcept {
            assert(node_ != nullptr);
            auto old_value(*this);
            node_ = node_->next_node;
            return old_value;
        }
 
     
        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);
            return node_->value;
        }
 
      
        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);
            return &node_->value;
        }
 
    private:
        Node* node_ = nullptr;
    };
 
public:
    SingleLinkedList() {
    }
 
  
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }
 
   
    [[nodiscard]] bool IsEmpty() const noexcept {
        return !(size_ != 0);
    }
 
    
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
 
    
    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            Node* new_head = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = new_head;
        }
        size_ = 0;
    }
 
    ~SingleLinkedList() {
        Clear();
    }
 
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
 
    
    using Iterator = BasicIterator<Type>;
 
    using ConstIterator = BasicIterator<const Type>;
 
    
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{ head_.next_node };
    }
 
  
    [[nodiscard]] Iterator end() noexcept {
        return Iterator{ nullptr };
    }
 
   
    [[nodiscard]] ConstIterator begin() const noexcept {
        return cbegin();
    }
    
    
    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator{ nullptr };
    }
 
  
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{ head_.next_node };
    }
 
   
    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{ nullptr };
    }
 
    SingleLinkedList(std::initializer_list<Type> values) {
    // используем шаблонный метод реализации
        Assign(values.begin(), values.end());
    }
 
    SingleLinkedList(const SingleLinkedList& other) {
    // используем шаблонный метод реализации
        Assign(other.begin(), other.end());
    }
 
    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs) {
            if (rhs.IsEmpty()) {
                Clear();
            } else {
                auto rhs_copy(rhs);
                swap(rhs_copy);
            }
        }
        return *this;
    }


    void swap(SingleLinkedList& other) noexcept {
        std::swap(other.head_.next_node, head_.next_node);
        std::swap(other.size_, size_);
    }
 
    
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{ &head_ };
    }
 
    
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        //возвращаем и преобразуем пустой указатель в пустой указатель целевого типа const_cast<тип>(ссылка)
        return ConstIterator{ const_cast<Node*>(&head_) };
    }
 
    
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return cbefore_begin();
    }
 
    
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);
        
        pos.node_->next_node = new Node(value, pos.node_->next_node);
        size_ = size_ + 1;
        return Iterator{ pos.node_->next_node };
    }
    
    void PopFront() noexcept {
        assert(!IsEmpty());
        
        Node* fresh_head = head_.next_node->next_node;
        delete head_.next_node;
        head_.next_node = fresh_head;
        size_ = size_ - 1;
    }
 
   
    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(!IsEmpty());
        
        Node* delete_pos = pos.node_->next_node->next_node;
        delete pos.node_->next_node;
        pos.node_->next_node = delete_pos;
        size_ = size_ - 1;
        return Iterator{ pos.node_->next_node };
    }
 
private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_ = 0;
    
    template <typename InputIterator>
    void Assign(InputIterator from, InputIterator to) {
        SingleLinkedList<Type> tmp;
    
        Node** node_ptr = &tmp.head_.next_node;
        while (from != to) {
            assert(*node_ptr == nullptr);
        
            *node_ptr = new Node(*from, nullptr);
            ++tmp.size_;
        
            node_ptr = &((*node_ptr)->next_node);
        
            ++from;
        }
        swap(tmp);
    }
};
 
template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}
 
template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    std::equal(lhs.begin(), lhs.end(),
               rhs.begin(), lhs.end());
    return true;
}
 
template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    !std::equal(lhs.begin(), lhs.end(),
                rhs.begin(), lhs.end());
    return true;
}
 
template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    return true;
}
 
template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}
 
template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return  rhs < lhs;
}
 
template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}