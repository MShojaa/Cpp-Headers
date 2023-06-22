#ifndef MSH_IO_H
#define MSH_IO_H

#include <iostream>
#include <string>
#include <fstream>
#include <tuple>
#include "vector.h"

#ifndef LOG
#define LOG(x) std::cout << x << std::endl
#endif
#ifndef Print
#define Print(x) std::cout << x << ' '
#endif

namespace msh
{
    class IO
    {
    // ------------------->     Variables     <-------------------
    private:
        msh::vector<msh::vector<double>> grid;
        size_t index;

    // ------------------->      Methods      <-------------------
    public:
        // -----> Constructors and Destructor <-----
        // Default Constructor
        IO() = default;
        // Destructor
        ~IO();

    // ------------------->   Other Methods   <-------------------
        // Read data from file
        template <typename... Args>
        size_t read(std::string &, Args&...);
        template <typename... Args>
        void write(std::string &, Args&...);
    private:
        // Release memory
        void reset();
        // Reset size to zero not releasing memory
        void soft_reset();
        // Variadic Template Assignment
        void variadic_assignment() {}
        template <typename Arg, typename... Args>
        void variadic_assignment(Arg&, Args&...);
    };

    // -----> Constructors and Destructor <-----
    // Destructor
    IO::~IO()
    {
        reset();
    }

    // ------------------->   Other Methods   <-------------------
    // Read data from file
    template <typename... Args>
    size_t IO::read(std::string &file_address, Args&... args)
    {
        std::fstream File;
        File.open(file_address, std::ios::in);
        if (!File.is_open())
        {
            std::cout << "Failed to open " << file_address << std::endl;
            exit(1);
        }

        size_t count {sizeof...(args)};
        grid.reserve(count);

        for (size_t i = 0; i < grid.capacity(); ++i)
            grid[i].reserve(1024);
        
        for (size_t i = 0, j = 0; File >> grid[i][j]; ++j)
        {
            for (i = 1; i < grid.capacity(); ++i)
                File >> grid[i][j];
            i = 0;
        }
        File.close();

        if (grid[0].size() > grid[1].size())
            grid[0].pop();

        index = 0;
        variadic_assignment(args...);

        size_t size_of_array = grid[0].size();
        // soft_reset();
        reset();

        return size_of_array;
    }
    // Write to file
    template <typename... Args>
    void IO::write(std::string &file_address, Args&... args)
    {
        std::fstream File;
        File.open(file_address, std::ios::out);
        if (!File.is_open())
        {
            std::cout << "Failed to open " << file_address << std::endl;
            exit(1);
        }

        auto first_arg = std::get<0>(std::tuple<Args*...>(&args...));
        size_t _size = first_arg->size();

        for (size_t i = 0; i < _size; ++i)
        {
            int n = 0;
            ((File << (n++ == 0 ? "" : " ") << args[i]), ...);
            File << "\n";
        }
        File.close();
    }
    // Release memory
    void IO::reset()
    {
        for (size_t i = 0; i < grid.capacity(); ++i)
            grid[i].reset();
        grid.reset();
    }
    // Reset size to zero not releasing memory
    void IO::soft_reset()
    {
        for (size_t i = 0; i < grid.capacity(); ++i)
            grid[i].reset_size();
        grid.reset_size();
    }
    // Variadic Template Assignment
    template <typename Arg, typename... Args>
    void IO::variadic_assignment(Arg &firsArg, Args&... args)
    {
        firsArg = grid[index];
        ++index;
        variadic_assignment(args...);
    }
} // namespace msh

#endif