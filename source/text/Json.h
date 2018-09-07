#pragma once

#include "Bag.h"

class Json {
    private:
        const char* input;
        int inputLength;
        int index;
        int lineNumber;
        int lastLineIndex;
        bool error;

        Json (const Text& _inputText) : input (_inputText), inputLength (_inputText.length ()), index (0), lineNumber (1), lastLineIndex (0), error (false) {}

    public:
        static PtrToBagThing read (const Text& filename) {
            Text text = File (filename).readText();
            Json json (text);
            PtrToBagThing bagThing;
            return bagThing;
        }
};
