#pragma once

#include        <sys/types.h>
#include        <sys/stat.h>
#include        "Text.h"

class File {
    private:
        Text path;
        struct stat stats;
        bool exists;

    public:
        File (const Text& _path) : path (_path), exists (stat (path, &stats) == 0) { }

        ~File () {}

        bool getExists () {
            return exists;
        }

        bool isDirectory () {
            return stats.st_mode | S_IFDIR;
        }
    }

};
