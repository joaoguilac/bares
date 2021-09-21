#ifndef _STACK_H_
#define _STACK_H_

#include <string>   // std::string
#include <iostream> // std::ostream
#include <memory>   // std::unique_ptr
#include <iterator> // std::advance, std::begin(), std::end(), std::ostream_iterator

/// Sequence stack container namespace.
namespace sta {

   /// Implements tha infrastrcture to support a bidirectional iterator.
   template < class T >
    class MyForwardIterator : public std::iterator<std::bidirectional_iterator_tag, T>
    {
        public:
            typedef MyForwardIterator self_type;   //!< Alias to iterator.
            // Below we have the iterator_traits common interface
            typedef std::ptrdiff_t difference_type; //!< Difference type used to calculated distance between iterators.
            typedef T value_type;           //!< Value type the iterator points to.
            typedef T* pointer;             //!< Pointer to the value type.
            typedef T& reference;           //!< Reference to the value type.
            typedef const T& const_reference;           //!< Reference to the value type.
            typedef std::bidirectional_iterator_tag iterator_category; //!< Iterator category.

            MyForwardIterator( pointer ptr = nullptr ) : m_ptr{ptr}  {};
            self_type& operator=( const self_type& other ) {
                m_ptr = other.m_ptr;
                return *this;
            }
            MyForwardIterator( const self_type& other ) : m_ptr{ other.m_ptr } {}
            reference operator*( ) const {
                return *m_ptr;
            }
            self_type& operator++( ) {
                m_ptr++;
                return *this;
            }; // ++it;
            self_type operator++( int ) {
                auto old {*this};
                m_ptr++;
                return old;
            }; // it++;

            self_type& operator--( ) {
                --m_ptr;
                return *this;
            }
            self_type operator--( int ) {
                auto old {*this};
                m_ptr--;
                return old;
            }
            
            friend self_type operator+( difference_type difference, self_type it) {
                return self_type{difference + it.m_ptr};
            };
            friend self_type operator+( self_type it, difference_type difference ) {
                return self_type{it.m_ptr + difference };
            };
            friend self_type operator-( self_type it, difference_type difference ) {
                return self_type{it.m_ptr - difference};
            }
            difference_type operator-( self_type it ) {
                return m_ptr - it.m_ptr;
            }
            bool operator==( const self_type& other) const {
                return other.m_ptr == m_ptr;
            };
            bool operator!=( const self_type& other) const {
                return other.m_ptr != m_ptr;
            };

        private:
            pointer m_ptr; //!< The raw pointer.
    };



    /// Represents a stack.
    /**
     * @brief This structure represents a stack, which is just a set of stacked data,
     * where we only work with its top.
     * We will use the stack data structure to store the received expression and be
     * able to calculate it.
     * @see The implementation was inspired by the website:
     * https://www.geeksforgeeks.org/stack-data-structure-introduction-program/
     * 
     * @tparam T the type of stack.
     */
    template <typename T>
    class stack
    {
        //=== Aliases
        public:
            using size_type = unsigned long; //!< The size type.
            using value_type = T;            //!< The value type.
            using pointer = value_type*;     //!< Pointer to a value stored in the container.
            using reference = value_type&;   //!< Reference to a value stored in the container.
            using const_reference = const value_type&; //!< Const reference to a value stored in the container.

            using iterator = MyForwardIterator< value_type >; //!< The iterator, instantiated from a template class.
            using const_iterator = MyForwardIterator< const value_type >; //!< The const_iterator, instantiated from a template class.

        public:
            stack(void) // constructor.
            : m_end {0},
                m_capacity {0},
                m_storage {new T[0]} {
            };

            /**
             * @brief Adds an item in the stack. If the stack is full, then it is said 
             * to be an Overflow condition.
             * @param element the element that will be add on the stack.
             * @return true if pushed successfully.
             */
            bool push(T element)
            {
                // Check if there is space for new element.
                if (m_end >= m_capacity) {
                    if (m_capacity == 0) m_capacity++;
                    else m_capacity *= 2;
                    // Allocates a new space
                    std::unique_ptr<T[]> new_storage {new T[m_capacity]};
                    // Copies values of the stack to the new storage
                    std::copy(begin(), end(), new_storage.get());

                    m_storage = std::move(new_storage);
                }
                // Insert the element.
                m_storage[m_end] = element;
                m_end++;
                return true;
            };
            
            /**
             * @brief Removes an item from the stack. The items are popped in the
             * reversed order in which they are pushed. If the stack is empty,
             * then it is return an error. 
             * @return T the element that was removed from the stack.
             */
            T pop(void)
            {
                if (m_end == 0) {
                    throw std::runtime_error("pop(): cannot use this method on an empty stack");
                }
                T temporary = m_storage[m_end - 1];
                m_end--;
                return temporary;
            };

            /**
             * @brief Get the top element of the stack.
             * @return T Returns top element of stack.
             */
            T top(void)
            {
                if (m_end == 0) {
                    throw std::runtime_error("top(): cannot use this method on an empty stack");
                }
                else {
                    return m_storage[m_end - 1];
                }
            };

            /**
             * @brief Check if the stack is empty.
             * @return true Returns true if stack is empty.
             * @return false otherwise.
             */
            bool empty(void)
            {
                return m_end == 0;
            };

            /**
             * @brief Get the stack size.
             * @return size_type the number of elements stored on the stack.
             */
            size_type size( void ) const {
                return m_end;
            }

        private:
            size_type m_end;                //!< The list's current size (or index past-last valid element).
            size_type m_capacity;           //!< The list's storage capacity.
            std::unique_ptr<T[]> m_storage; //!< The list's data storage area.
            
            //=== [II] ITERATORS
            /**
            * @return an iterator to the begin of the stack
            */
            iterator begin( void ) {
                return iterator{m_storage.get()};
            };
            /**
            * @return an iterator to the position after the end of the stack
            */
            iterator end( void ) {
                return iterator{m_storage.get() + m_end};
            };
    };
}

#endif
