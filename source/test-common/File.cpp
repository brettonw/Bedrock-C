#include "Test.h"
#include "File.h"

TEST_CASE(TestFileExists) {
    File    file ("test.txt");
    TEST_XY(file.getExists (), true);

    File    directory ("..");
    TEST_XY(directory.getExists (), true);

    File    junk ("nonexistent-file.txt");
    TEST_XY(junk.getExists (), false);
}

TEST_CASE(TestFileBasename) {
    File    file ("./test.txt");
    TEST_XY(file.getPath (), "./test.txt");
    TEST_XY(file.getBasename (), "test");
    TEST_XY(file.getExtension (), "txt");

    File file2 ("./dir/dir2");
    TEST_XY(file2.getExists (), true);
    TEST_XY(file2.isDirectory (), true);
    TEST_XY(file2.getPath (), "./dir/dir2");
    TEST_XY(file2.getBasename (), "dir2");
    TEST_XY(file2.getExtension (), "");

    File file3 ("./dir/dir2/test.long.txt");
    TEST_XY(file3.getExists (), true);
    TEST_XY(file3.getPath (), "./dir/dir2/test.long.txt");
    TEST_XY(file3.getBasename (), "test.long");
    TEST_XY(file3.getExtension (), "txt");
}

TEST_CASE(TestFileIsDirectory) {
    //Log::Scope scope (Log::DEBUG);

    File    file ("test.txt");
    TEST_XY(file.isDirectory (), false);

    File    directory ("..");
    TEST_XY(directory.isDirectory (), true);

    File    junk ("nonexistent-file.txt");
    TEST_XY(junk.isDirectory (), false);

    vector<PtrToFile> files =  directory.getFiles ();
    TEST_XYOP(files.size (), 0, >);
    TEST_X((files[0]->getBasename () == "debug") || (files[0]->getBasename () == "release"));

    Log::debug () << files.size () << " files in directory (" << directory.getPath () << ")" << endl;
}

