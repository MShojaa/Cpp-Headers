#ifndef MSH_VECTOR_H
#define MSH_VECTOR_H

#include <iostream>
#include <cstring>

#ifndef LOG
#define LOG(x) std::cout << x << std::endl
#endif
#ifndef Print
#define Print(x) std::cout << x << ' '
#endif

namespace msh
{
    // ----------------------------->     ARRAY     <-----------------------------
    template <typename T>
    class array;
    // ----------------------------->     VECTOR     <-----------------------------
    // template <typename T>
    // class vector;

    // ----------------------------->     VECTOR     <-----------------------------
    template <typename T>
    class vector
    {
    // ------------------->     Variables     <-------------------
    private:
        T *_array;
        size_t _size;
        size_t _capacity;

    // ------------------->      Methods      <-------------------
    public:
        // -----> Constructors and Destructor <-----
        // Default Constructor
        vector() : _array(nullptr),  _size(0), _capacity(0) {}
        // Constructor that creates a dynamic allocated vector using it's capacity
        vector(const size_t _capacity) : _array(new T[_capacity]),  _size(0), _capacity(_capacity) {}
        // Destructor
        ~vector();

        // -----> Getters <-----
        // Get array
        T *get() const;
        // Get size
        size_t size() const;
        // Get capacity
        size_t capacity() const;

        // -----> Operators Overloading <-----
        // Assignment Operator
        vector &operator=(const vector &);
        // Bracket(Index) Operator
        T &operator[](const size_t);
        // Reference Operator
        T *operator&() const;

        // -----> Other Methods <-----
        // Reserve a block of memory
        void reserve(const size_t);
        // Remove last element
        void pop();
        // Reset size
        void reset_size();
        // Reset to nullptr
        void reset();
    private:
        // Reallocate block of memory
        void realloc(const size_t);
        // Copy elements
        void copy_elements(T *const);
        // Shrink capacity to size and release extra memory
        // void shrink_to_fit();
    };

    // -----> Constructors and Destructor <-----
    // Destructor
    template <typename T>
    vector<T>::~vector()
    {
        delete[] _array;
    }

    // -----> Getters <-----
    // Get array
    template <typename T>
    T *vector<T>::get() const
    {
        return _array;
    }
    // Get size
    template <typename T>
    inline size_t vector<T>::size() const
    {
        return _size;
    }
    // Get capacity
    template <typename T>
    inline size_t vector<T>::capacity() const
    {
        return _capacity;
    }

    // -----> Operators Overloading <-----
    // Assignment Operator
    template <typename T>
    vector<T> &vector<T>::operator=(const vector<T> &other)
    {
        if (_array != other._array)
        {
            _size = other._size;
            if (_capacity != other._capacity)
            {
                _capacity = other._capacity;
                if (_array)
                    delete[] _array;
                _array = new T[_capacity];
            }
            copy_elements(other._array);
        }
        return *this;
    }
    // Bracket(Index) Operator
    template <typename T>
    inline T &vector<T>::operator[](const size_t index)
    {
        if (index == _size)
            ++_size;
        if (_size > _capacity)
            realloc(2 * _capacity);

        return _array[index];
    }
    // Reference Operator
    template <typename T>
    inline T *vector<T>::operator&() const
    {
        return _array;
    }

    // -----> Other Methods <-----
    // Reserve a block of memory
    template <typename T>
    void vector<T>::reserve(const size_t _capacity)
    {
        if (_array)
            realloc(_capacity);
        else
        {
            this->_capacity = _capacity;
            _array = new T[_capacity];
        }
    }
    // Remove last element
    template <typename T>
    inline void vector<T>::pop()
    {
        --_size;
    }
    // Reset size
    template <typename T>
    void vector<T>::reset_size()
    {
        _size  = 0;
    }
    // Reset to nullptr
    template <typename T>
    void vector<T>::reset()
    {
        if (_array)
        {
            delete[] _array;
            _array = nullptr;
        }
        _size     = 0;
        _capacity = 0;
    }
    // Reallocate block of memory
    template <typename T>
    void vector<T>::realloc(const size_t _capacity)
    {
        if (this->_capacity != _capacity)
        {
            this->_capacity = _capacity;
            if (_size > _capacity)
                _size = _capacity;

            T *temp = _array;
            _array = new T[_capacity];
            copy_elements(temp);
            delete[] temp;
        }
    }
    // Copy elements
    template <typename T>
    void vector<T>::copy_elements(T *const other_array)
    {
        for (size_t i = 0; i < _size; ++i)
            _array[i] = other_array[i];
    }
    // Shrink capacity to size and release extra memory
    // template <typename T>
    // void vector<T>::shrink_to_fit()
    // {
    //     if (_size > 0 && _size < _capacity)
    //         realloc(_size);
    // }

    // ----------------------------->     ARRAY     <-----------------------------
    template <typename T>
    class array
    {
    // ------------------->     Variables     <-------------------
    private:
        T *_array;
        size_t _size;
    // ------------------->      Methods      <-------------------
    public:
        // -----> Constructors and Destructor <-----
        // Default Constructor
        array() : _array(nullptr), _size(0) {}
        array(const size_t _size) : _array(new T[_size]), _size(_size) {}
        // Destructor
        ~array();

        // -----> Getters <-----
        // Get size
        size_t size() const;

        // -----> Operators Overloading <-----
        // Assignment Operator
        array<T> &operator=(const vector<T> &);
        // Bracket(Index) Operator
        T &operator[](size_t);
    };

    // -----> Constructor and Destructor <-----
    // Destructor
    template <typename T>
    array<T>::~array()
    {
        delete[] _array;
    }

    // -----> Getters <-----
    // Get size
    template <typename T>
    inline size_t array<T>::size() const
    {
        return _size;
    }

    // -----> Operators Overloading <-----
    // Assignment Operator
    template <typename T>
    array<T> &array<T>::operator=(const vector<T> &other)
    {
        if (_array != other.get())
        {
            if (_size != other.size())
            {
                _size = other.size();
                if (_array)
                    delete[] _array;
                _array = new T[_size];
            }
            std::memcpy(_array, other.get(), _size * sizeof(T));
        }
        return *this;
    }
    // Bracket(Index) Operator
    template <typename T>
    T &array<T>::operator[](size_t index)
    {
        return _array[index];
    }

} // namespace msh

#endif