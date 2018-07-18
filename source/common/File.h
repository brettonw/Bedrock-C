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
            return exists and S_ISDIR(stats.st_mode);
        }

        vector<File> getFiles () {
            if (isDirectory ()) {
                vector<File> result;
                //FILE*   directory = opendir
                return result;
            }
            Text out;
            out << "File (" << path << ") is not a directory";
            throw runtime_error (out);
        }

        // open, close
        // read, write
        // readText, writeText
        // read directory
        // delete
        // rename

};
