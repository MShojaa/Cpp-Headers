#ifndef MSH_IO_H
#define MSH_IO_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "vector.h"

#ifndef LOG
#define LOG(x) std::cout << x << std::endl
#endif
#ifndef Print
#define Print(x) std::cout << x << ' '
#endif

namespace msh
{
    template <typename T>
    class IO
    {
    // ------------------->     Variables     <-------------------
    private:
        std::string file_address;
        msh::vector<msh::vector<T>> grid;
        size_t index;

    // ------------------->      Methods      <-------------------
    public:
        // -----> Constructors and Destructor <-----
        // Default Constructor
        IO() = default;
        // Constructor that set file_address
        explicit IO(std::string &file_address) : file_address(file_address) {}
        // Constructor that set file_address
        explicit IO(const char *file_address) : file_address(file_address) {}
        // Destructor
        // ~IO();
    private:
        // -----> Setters <-----
        // Set file address
        void set_file_address(std::string &);
        // Set file address
        void set_file_address(const char *);
    public:
        // -----> Operators Overloading <-----
        // Assignment Operator
        void operator=(std::string &);
        void operator=(const char *);

    // ------------------->   Other Methods   <-------------------
        // Read data from file
        template <typename... Args>
        size_t read_file(Args&... args);
        // Release memory
        void reset();
        void soft_reset();
    private:
        // Variadic Template Assignment
        // template <typename... Args>
        // void variadic_assignment(msh::vector<msh::vector<T>>&, T&, Args&...);
        template <typename Arg, typename... Args>
        void variadic_assignment(Arg&, Args&...);
        void variadic_assignment(){}
    };

    // -----> Operators Overloading <-----
    // Assignment Operator
    template <typename T>
    void IO<T>::operator=(std::string &file_address)
    {
        set_file_address(file_address);
    }
    template <typename T>
    void IO<T>::operator=(const char *file_address)
    {
        set_file_address(file_address);
    }

    // -----> Setters <-----
    // Set file address
    template <typename T>
    void IO<T>::set_file_address(std::string& file_address)
    {
        this->file_address = file_address;
    }
    // Set file address
    template <typename T>
    void IO<T>::set_file_address(const char *file_address)
    {
        this->file_address = file_address;
    }

    // ------------------->   Other Methods   <-------------------
    // Read data from file
    template <typename T>
    template <typename... Args>
    size_t IO<T>::read_file(Args&... args)
    {
        std::fstream File;
        File.open(file_address, std::ios::in);
        if (!File.is_open())
        {
            std::cout << "Error opening the " << file_address << " file!" << std::endl;
            exit(1);
        }
        
        grid.reserve(8);
        std::string first_line;
        std::getline(File, first_line);
        std::stringstream ss(first_line);
        std::string num_str;
        size_t i{}, j{};
        while (std::getline(ss, num_str, ' '))
        {
            grid[i].reserve(1024);
            grid[i][j] = static_cast<T>(std::stod(num_str));
            ++i;
        }

        for (i = 0, j = 1; File >> grid[i][j]; ++j)
        {
            for (i = 1; i < grid.size(); ++i)
                File >> grid[i][j];
            i = 0;
        }
        if (grid[0].size() > grid[1].size())
            grid[0].pop();

        for (size_t i = 0; i < grid.size(); ++i)
        {
            for (size_t j = 0; j < grid[i].size(); ++j)
                Print(grid[i][j]);
            LOG("");
        }
        index = 0;
        variadic_assignment(args...);

        size_t size_of_array = grid[0].size();
        soft_reset();

        return size_of_array;
    }
    // Release memory
    template <typename T>
    void IO<T>::reset()
    {
        for (size_t i = 0; i < grid.capacity(); ++i)
            grid[i].reset();
        grid.reset();
    }
    // Reset size to zero not releasing memory
    template <typename T>
    void IO<T>::soft_reset()
    {
        for (size_t i = 0; i < grid.capacity(); ++i)
            grid[i].reset_size();
        grid.reset_size();
    }
    // Variadic Template Assignment
    template <typename T>
    template <typename Arg, typename... Args>
    void IO<T>::variadic_assignment(Arg &firsArg, Args&... args)
    {
        firsArg = grid[index];
        ++index;
        variadic_assignment(args...);
    }

} // namespace msh

#endif