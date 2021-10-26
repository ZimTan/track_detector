#include <cerrno>
#include <iostream>
#include <sys/stat.h>
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
    struct stat st = {0};
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

int main(int argc, char **argv)
{
    if (args_check(argc, argv) == 0)
        return 1;

    if (create_result_dir(argv[1]) == 0)
        return errno;

    cnpy::npy_load("/home/tanz/Bureau/data_sealcar/32.npy");
    return 0;
}
