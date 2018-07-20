#pragma once

#include        <sys/types.h>
#include        <sys/stat.h>
#include        <dirent.h>
#include        "Text.h"

MAKE_PTR_TO(File) {
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

        vector<PtrToFile> getFiles () {
            if (isDirectory ()) {
                vector<PtrToFile> files;
                DIR* directory = opendir (path);
                if (directory) {
                    struct dirent* ent;
                    while ((ent = readdir (directory))) {
                        files.push_back (new File (ent->d_name));
                    }
                    closedir (directory);
                }
                return files;
            }
            throw runtime_error (Text ("File (") << path << ") is not a directory");
        }

        // open, close
        // read, write
        // readText, writeText
        // read directory
        // delete
        // rename

};
