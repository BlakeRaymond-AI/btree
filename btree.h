#pragma once
#include <vector>
#include <list>
#include <memory>
#include <iterator>

#define BLOCK_SIZE 3

template <typename T>
class block_vector_iterator 
    : public std::iterator<std::bidirectional_iterator_tag, T>
{
    typedef block_vector_iterator<T> self_type;
    typedef typename std::list<std::vector<typename T::value_type>>::iterator block_iter_type;
    typedef typename std::vector<typename T::value_type>::iterator item_iter_type;

public:
    block_vector_iterator(block_iter_type b, item_iter_type i)
        : _block_iter(b)
        , _item_iter(i)
    {}

    typename T::reference operator*()
    {
        return *_item_iter;
    }
    
    typename T::const_reference operator*() const
    {
        return *_item_iter;
    }

    typename T::pointer operator->()
    {
        return _item_iter.operator->();
    }

    typename T::const_pointer operator->() const
    {
        return _item_iter.operator->();
    }

    bool operator==(const self_type& other) const
    {
        return _block_iter == other._block_iter
            && _item_iter == other._item_iter;
    }

    bool operator!=(const self_type& other) const
    {
        return !(*this == other);
    }

    self_type& operator++()
    {
        if (++_item_iter == _block_iter->end())
        {
            _item_iter = (++_block_iter)->begin();
        }
        return *this;
    }

    self_type& operator--()
    {
        if (_item_iter == _block_iter->begin())
        {
            _item_iter = (--_block_iter)->end();
        }
        --_item_iter;
        return *this;
    }

private:
    typename std::list<std::vector<typename T::value_type>>::iterator _block_iter;
    typename std::vector<typename T::value_type>::iterator _item_iter;
};

template <typename T>
class block_vector
{
    typedef block_vector<T> self_type;

public:
    typedef typename std::vector<T>::value_type value_type;
    typedef typename std::vector<T>::pointer pointer;
    typedef typename std::vector<T>::const_pointer const_pointer;
    typedef typename std::vector<T>::reference reference;
    typedef typename std::vector<T>::const_reference const_reference;
    typedef typename std::vector<T>::size_type size_type;
    typedef typename std::vector<T>::difference_type difference_type;

    typedef block_vector_iterator<self_type> iterator;

private:
    typename std::list<std::vector<T>>::iterator last()
    {
        return std::prev(_blocks.end(), 2);
    }

    typename std::list<std::vector<T>>::const_iterator last() const
    {
        return std::prev(_blocks.end(), 2);
    }

    typename std::list<std::vector<T>>::iterator get_not_full_block()
    {
        auto block = last();
        if (block->size() == BLOCK_SIZE)
        {
            block = _blocks.emplace(std::next(block));
        }
        return block;
    }

public:
    block_vector()
        : _size(0)
    {
        _blocks.emplace_back();
        _blocks.emplace_back();
    }

    iterator begin()
    {
        return iterator(_blocks.begin(), _blocks.front().begin());
    }

    iterator end()
    {
        return iterator(std::prev(_blocks.end()), _blocks.back().end());
    }

    reference front()
    {
        return _blocks.front().front();
    }

    reference back()
    {
        return std::prev(_blocks.end(), 2)->back();
    }

    const_reference front() const
    {
        return _blocks.front().front();
    }

    const_reference back() const
    {
        return std::prev(_blocks.end(), 2)->back();
    }

    void clear()
    {
        _blocks.clear();
        _blocks.emplace_back();
        _blocks.emplace_back();
        _size = 0;
    }

    void push_back(const_reference value)
    {
        auto block = get_not_full_block();
        block->push_back(value);
        ++_size;
    }

    template <class... Args>
    void emplace_back(Args&&... args)
    {
        auto block = get_not_full_block();
        block->emplace_back(args...);
        ++_size;
    }

    size_t size() const
    {
        return _size;
    }

    bool empty() const
    {
        return _size == 0;
    }

private:
    std::list<std::vector<T>> _blocks;
    size_t _size;
};

