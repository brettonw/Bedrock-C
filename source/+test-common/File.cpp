#include "Test.h"
#include "File.h"

TEST_CASE(FileExists) {
    File file ("test.txt");
    TEST_EQUALS(file.getExists (), true);

    File directory ("..");
    TEST_EQUALS(directory.getExists (), true);

    File junk ("nonexistent-file.txt");
    TEST_EQUALS(junk.getExists (), false);
}

TEST_CASE(FileBasename) {
    File file ("./test.txt");
    TEST_EQUALS(file.getPath (), "./test.txt");
    TEST_EQUALS(file.getBasename (), "test");
    TEST_EQUALS(file.getExtension (), "txt");

    File file2 ("./dir/dir2");
    TEST_EQUALS(file2.getExists (), true);
    TEST_EQUALS(file2.isDirectory (), true);
    TEST_EQUALS(file2.getPath (), "./dir/dir2");
    TEST_EQUALS(file2.getBasename (), "dir2");
    TEST_EQUALS(file2.getExtension (), "");

    File file3 ("./dir/dir2/test.long.txt");
    TEST_EQUALS(file3.getExists (), true);
    TEST_EQUALS(file3.getPath (), "./dir/dir2/test.long.txt");
    TEST_EQUALS(file3.getBasename (), "test.long");
    TEST_EQUALS(file3.getExtension (), "txt");
}

TEST_CASE(FileIsDirectory) {
    //Log::Scope scope (Log::DEBUG);

    File file ("test.txt");
    TEST_EQUALS(file.isDirectory (), false);

    File directory ("..");
    TEST_EQUALS(directory.isDirectory (), true);

    File junk ("nonexistent-file.txt");
    TEST_EQUALS(junk.isDirectory (), false);

    vector<PtrToFile> files =  directory.getFiles ();
    TEST_XYOP(files.size (), 0, >);
    TEST_TRUE((files[0]->getBasename () == "debug") || (files[0]->getBasename () == "release"));

    Log::debug () << files.size () << " files in directory (" << directory.getPath () << ")" << endl;
}

TEST_CASE(FileRead) {
    File    file ("test.txt");
    PtrToBuffer buffer = file.read ();
    byte compare[] = { 'T', 'e', 's', 't', '\n' };
    TEST_EQUALS(buffer->getLength (), 5);
    TEST_EQUALS(buffer->compare (compare, 5), 0);
}

TEST_CASE(FileReadText) {
    File file ("test.txt");
    Text text = file.readText ();
    TEST_EQUALS(text, "Test\n");
}
