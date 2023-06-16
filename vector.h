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
    template <typename T>
    class shared_vector
    {
    // ------------------->     Variables     <-------------------
    private:
        T *_array;
        T *temp_array;
        size_t *_count;
        size_t *_size;
        size_t *_capacity;
        bool *allowIndexOutOfBound{new bool(false)};

    // ------------------->      Methods      <-------------------
    public:
        // -----> Constructors and Destructor <-----
        // Default constructor
        shared_vector();
        // Constructor that creates a dynamic allocated vector using it's capacity
        shared_vector(size_t);
        // Constructor that takes a pointer to a dynamically allocated object
        explicit shared_vector(T*);
        // Copy constructor
        shared_vector(const shared_vector &);

        // Destructor
        ~shared_vector();

        // -----> Getters <-----
        // Get count
        size_t use_count();
        // Get size
        size_t size();
        // Get capacity
        size_t capacity();
        // Get permission situation whether to add new element to the array or not
        bool get_permission();

        // -----> Setters <-----
        // To change permission to add new element to the array
        void set_permission(bool);

        // -----> Operators Overloading <-----
        // Assignment operator
        shared_vector &operator=(const shared_vector &);
        // Bracket(Index) operator
        T &operator[](size_t);
        // Dereference operator
        T &operator*() const;
        // Arrow operator
        T *operator->() const;
        // Get the pointer to the underlying object
        T *get() const;

        // -----> Other Methods <-----
        // Reserve some amount of blocks
        void reserve(size_t);
        // Shrink capacity to size
        void shrink();
        void shrink(size_t);
        // Add elements indirectly
        void emplace_back(T&);

    private:
        // Release the shared object and decrement the reference count
        void release();
        // Reallocate the array
        void realloc(size_t _capacity = 0);
        // Swap the temperory array to _array
        void swap();
    };

    // -----> Constructors and Destructor <-----
    template <typename T>
    shared_vector<T>::shared_vector() : _array(nullptr), _count(nullptr), _size(nullptr), _capacity(nullptr) {}

    template <typename T>
    shared_vector<T>::shared_vector(size_t count) : _array(new T[count]), _count(new size_t(1)), _size(new size_t(0)), _capacity(new size_t(count)) {}

    template <typename T>
    shared_vector<T>::shared_vector(T *ptr) : _array(ptr), _count(new size_t(1)), _size(new size_t(0)), _capacity(new size_t(1)) {}

    template <typename T>
    shared_vector<T>::shared_vector(const shared_vector &other) : _array(other._array), _count(other._count), _size(other._size), _capacity(other._capacity), allowIndexOutOfBound(other.allowIndexOutOfBound)
    {
        if (_count)
            ++(*_count);
    }

    // Destructor
    template <typename T>
    shared_vector<T>::~shared_vector()
    {
        set_permission(false);
        release();
    }

    // -----> Getters <-----
    // Get count
    template <typename T>
    size_t shared_vector<T>::use_count()
    {
        return (_count ? *_count : 0);
    }

    // Get size
    template <typename T>
    size_t shared_vector<T>::size()
    {
        return (_size ? *_size : 0);
    }

    // Get capacity
    template <typename T>
    size_t shared_vector<T>::capacity()
    {
        return (_capacity ? *_capacity : 0);
    }

    // Get permission situation whether to add new element to the array or not
    template <typename T>
    bool shared_vector<T>::get_permission()
    {
        return *allowIndexOutOfBound;
    }

    // -----> Setters <-----
    // To change permission to add new element to the array
    template <typename T>
    void shared_vector<T>::set_permission(bool flag)
    {
        *allowIndexOutOfBound = flag;
    }

    // -----> Operators Overloading <-----
    // Assignment operator
    template <typename T>
    shared_vector<T>& shared_vector<T>::operator=(const shared_vector &other)
    {
        if (this != &other)
        {
            release();
            _array    = other._array;
            _count    = other._count;
            _size     = other._size;
            _capacity = other._capacity;
            allowIndexOutOfBound = other.allowIndexOutOfBound;
            if (_count)
                ++(*_count);
        }
        return *this;
    }

    // Bracket(Index) operator
    template <typename T>
    T& shared_vector<T>::operator[](size_t index)
    {
        // LOG("size: " << *_size);
        // if (!_count)
        // {
        //     _array = new T[1];
        //     _count = new size_t(1);
        //     _size = new size_t(0);
        //     _capacity = new size_t(1);
        // }

        if (index < *_size)
        {
            return _array[index];
        }
        else if(*allowIndexOutOfBound)
        {
            if (index > *_size)
            {
                LOG("index out of bound\nProgram terminated");
                exit(1);
            }
            else
            {
                ++(*_size);
                if (*_size > *_capacity)
                    realloc();
                return _array[index];
            }
        }
        else
        {
            LOG("Use emplace_back() function\nProgram terminated");
            exit(1);
        }
    }

    // Dereference operator
    template <typename T>
    T& shared_vector<T>::operator*() const
    {
        return *_array;
    }

    // Arrow operator
    template <typename T>
    T* shared_vector<T>::operator->() const
    {
        return _array;
    }

    // Get the pointer to the underlying object
    template <typename T>
    T* shared_vector<T>::get() const
    {
        return _array;
    }

    // -----> Other Methods <-----
    // Reserve some amount of blocks
    template <typename T>
    void shared_vector<T>::reserve(size_t _capacity)
    {
        realloc(_capacity);
    }

    // Shrink capacity to size
    template <typename T>
    void shared_vector<T>::shrink()
    {
        if (*_capacity > *_size)
        {
            *_capacity = *_size;
            temp_array = new T[*_size];
            std::memcpy(temp_array, _array, *_size * sizeof(T));
            swap();
        }
    }

    template <typename T>
    void shared_vector<T>::shrink(size_t _count)
    {
        for (size_t i = 0; i < _count; ++i)
        {
            if (*(this + i)->_capacity > *(this + i)->_size)
            {
                *(this + i)->_capacity = *(this + i)->_size;
                temp_array = new T[*_size];
                std::memcpy(temp_array, _array, *_size * sizeof(T));
                swap();
            }
        }
    }

    // Add elements indirectly
    template <typename T>
    void shared_vector<T>::emplace_back(T &element)
    {
        _array[*_size] = element;
        ++(*_size);

        if (*_size > *_capacity)
            realloc();
    }

    // Release the shared object and decrement the reference count
    template <typename T>
    void shared_vector<T>::release()
    {
        if (_count)
        {
            --(*_count);
            if (*_count == 0)
            {
                delete _array;
                delete _count;
                delete _size;
                delete _capacity;
                delete allowIndexOutOfBound;
            }
            _array    = nullptr;
            _count    = nullptr;
            _size     = nullptr;
            _capacity = nullptr;
            allowIndexOutOfBound = nullptr;
        }
    }

    // Reallocate the array
    template <typename T>
    void shared_vector<T>::realloc(size_t _capacity)
    {
        if (!_count)
        {
            this->_array    = new T[1];
            this->_count    = new size_t(1);
            this->_size     = new size_t(0);
            this->_capacity = new size_t(1);
        }

        if (_capacity == 0)
        {
            *(this->_capacity) += 1024;
            // LOG("capacity: " << *(this->_capacity));
        }
        else
        {
            if (*(this->_size) > _capacity)
                *(this->_size) = _capacity;
            *(this->_capacity) = _capacity;
        }

        temp_array = new T[_capacity];
        std::memcpy(temp_array, _array, *(this->_size) * sizeof(T));
        swap();
    }

    // Swap the temperory array to _array
    template <typename T>
    void shared_vector<T>::swap()
    {
        T *temp_ptr = _array;
        _array = temp_array;
        delete[] temp_ptr;
        temp_array = nullptr;
    }
} // namespace msh

#endif