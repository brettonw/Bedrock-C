#pragma once

#ifdef __APPLE__
#include <libproc.h>
// libproc pulls in something that defines these values
#undef TRUE
#undef FALSE
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>
#include <unistd.h>

#include "Log.h"
#include "Text.h"

MAKE_PTR_TO(File) {
    private:
        Text path;
        struct stat stats;
        bool exists;

        template<typename BufferType>
        PtrTo<BufferType> readIntoBuffer () const {
            PtrTo<BufferType> buffer;

            // open the file with the file pointer at the end of the file, and check if that worked
            ifstream file (path, ios::in | ios::binary | ios::ate);
            if (file.is_open()) {
                // get the size of the file and create the buffer to hold it,
                streampos size = file.tellg ();
                buffer = BufferType::make (size);

                // go back to the beginning of the file and read the contents into the buffer
                file.seekg (0, ios::beg);
                file.read ((char*) buffer->fill (size), size);

                // close the file before returning
                file.close();
            }
            return buffer;
        }

        static Text dirname (Text path, const char* separator = "/") {
            // find the last "/"
            int last;
            int current = -1;
            do  {
                last = current;
                current = path.find (separator, current + 1);
            } while (current >= 0);
            return (last >= 0) ? path.substring(0, last + 1) : path;
        }

    public:
        File (const Text& _path) : path (_path), exists (stat (path, &stats) == 0) { }

        ~File () {}

        bool getExists () const {
            return exists;
        }

        bool isDirectory () const {
            return exists and S_ISDIR(stats.st_mode);
        }

        vector<PtrToFile> getFiles () const {
            if (isDirectory ()) {
                vector<PtrToFile> files;
                DIR* directory = opendir (path);
                if (directory) {
                    struct dirent* ent;
                    while ((ent = readdir (directory))) {
                        Text name (ent->d_name);
                        if (name.find (".") != 0) {
                            name = (path + "/") << name;
                            Log::trace () << "Found file in directory (" << path << "): " << name << endl;
                            files.push_back (new File (name));
                        }
                    }
                    closedir (directory);
                }
                sort (files.begin (), files.end ());
                return files;
            }
            throw RuntimeError (Text ("File (") << path << ") is not a directory");
        }

        Text getPath () const {
            return path;
        }

        PtrToFile getDirectory () const {
            return new File (dirname (path));
        }

        Text getBasename () const {
            vector<Text> components = path.split ("/");
            Text basename = components.back ();
            vector<Text> nameComponents = basename.split (".");
            if (nameComponents.size () > 1) {
                uint remove = nameComponents.back ().length () + 1;
                basename = basename.substring (0, basename.length () - remove);
            }
            return basename;
        }

        Text getExtension () const {
            Text extension;
            vector<Text> components = path.split (getBasename () + ".");
            if (components.size () > 1) {
                extension = components.back ();
            }
            return extension;
        }

        PtrToBuffer read () const {
            return readIntoBuffer<Buffer> ();
        }

        Text readText () const {
            return Text (readIntoBuffer<RawText> ());
        }

        void write (const char* ptr, long length) {
            ofstream file (path, ios::out | ios::binary | ios::trunc);
            if (file.is_open()) {
                file.write (ptr, length);

                // close the file before returning
                file.close();
            }
        }

        void writeText (const Text& text) {
            write (text.get(), text.length ());
        }

        static PtrToFile getExecutable () {
            char buffer[PATH_MAX];
            #ifdef __linux
            ssize_t length = readlink("/proc/self/exe", buffer, PATH_MAX);
            #endif
            #ifdef __APPLE__
            int length = proc_pidpath (getpid (), buffer, PATH_MAX);
            #endif
            return (length != -1) ? new File (Text (buffer, length)) : PtrToFile ();
        }


        // open, close
        // read, write
        // readText, writeText
        // read directory
        // delete
        // rename

};
