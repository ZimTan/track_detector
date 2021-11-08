#include <cerrno>
#include <iostream>
#include <thread>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "cnpy.h"
#include "track_detector.hh"

inline bool dir_check(const std::string &dir)
{
    struct stat buffer;
    return (stat (dir.c_str(), &buffer) == 0);
}

// Check that the program is given a path to a valid directory.
int args_check(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Error : Too few arguments : Expected 2 got : "
                  << argc << ".\n";
        return 0;
    }

    if (argc > 2)
    {
        std::cerr << "Error : Too many arguments : Expected : 2 got : "
                  << argc << ".\n";;
        return 0;
    }

    if (dir_check(argv[1]) == false)
    {
        std::cerr << "Error : Directory : '"
                  << argv[1] << "' does not exist.\n";
        return 0;
    }

    return 1;
}

// Create the directory where the segmented image will be saved
int create_result_dir(const std::string &dir)
{
    const std::string res_dir = dir + "frag";

    if (dir_check(res_dir) == false)
        if (mkdir(res_dir.c_str(), 0700) == -1)
        {
            std::cerr << "Error : Unable to create result directory : "
                      << res_dir << ".\n";
            return 0;
        }
    return 1;
}

uint8_t get_average(uint8_t *data, const size_t r, const size_t cols)
{
    size_t avg = 0;

    for (size_t i = 0; i < cols * 3; ++i)
        avg += data[r + i];

    return avg / (cols * 3);
}

void darken(uint8_t *data, const size_t cols, const size_t begin, const size_t end)
{
    for (size_t r = begin; r < end * cols * 3; r += cols * 3)
    {
        for (size_t c = 0; c < cols * 3; ++c)
        {
            data[r + c++] = 0;
            data[r + c++] = 0;
            data[r + c] = 0;
        }
    }
}

void process(uint8_t *data, size_t r, const size_t begin, const size_t end, uint8_t avg)
{
    for (size_t c = begin; c < end; ++c)
    {
        if (data[r + c] * 0.02125 + data[r + c + 1] * 0.7154 + data[r + c + 2] * 0.0721
            < avg)
        {
            data[r + c++] = 0;
            data[r + c++] = 0;
            data[r + c] = 0;
        }
        else
        {
            data[r + c++] = 255;
            data[r + c++] = 255;
            data[r + c] = 255;
        }
    }
}

void detect(const std::string &path, const std::string &file)
{
    cnpy::NpyArray arr = cnpy::npy_load(path + file);
    uint8_t *data = arr.data<uint8_t>();
    std::vector<size_t> shape = arr.shape;
    const size_t rows = shape[0];
    const size_t cols = shape[1];

    if (arr.word_size == sizeof(uint8_t))
    {
        darken(data, cols, 0, 200);
        darken(data, cols, 350 * cols * 3, rows);

        for (size_t r = 200 * cols * 3; r < 350 * cols * 3; r += cols * 3)
        {
            uint8_t average = get_average(data, r, cols) - 15;
            process(data, r, 0, cols * 3, average);
        }
    }
    const std::string res_path = path + "frag/" + file;
    cnpy::npy_save(res_path, data, {rows, cols, 3}, "w");
}

int main(int argc, char **argv)
{
    if (args_check(argc, argv) == 0)
        return 1;

    if (create_result_dir(argv[1]) == 0)
        return errno;

    DIR *dir = opendir(argv[1]);
    if (dir != NULL)
    {
        struct dirent *f = readdir(dir);
        while (f != NULL)
        {
            if (strstr(f->d_name, ".npy") != NULL)
                detect(argv[1], f->d_name);
            f = readdir(dir);
        }
    }

    return 0;
}
