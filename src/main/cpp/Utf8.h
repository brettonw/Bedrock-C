#include    "Types.h"

#pragma once

class Utf8 {
    private:
        const char* position;
        const char* next;
        int codePoint;

        // this UTF-8 decoding is modelled after the javascript at:
        //     https://github.com/mathiasbynens/utf8.js/blob/master/utf8.js
        // see https://encoding.spec.whatwg.org/#utf-8 for the UTF-8 specification
        int readContinuationByte () {
            if (*next) {
                byte continuationByte = *next++;
                if ((continuationByte & 0xC0) == 0x80) {
                    return continuationByte & 0x3F;
                }
            }
            // error case
            throw runtime_error ("Invalid continuation byte");
        }

        void readNext () {
            try {
                uint byte1;
                uint byte2;
                uint byte3;
                uint byte4;
                if (*next == 0) {
                    codePoint = 0;
                    return;
                }

                // Read first byte
                byte1 = *next++;

                // 1-byte sequence (no continuation bytes)
                if ((byte1 & 0x80) == 0) {
                    codePoint = byte1;
                    return;
                }

                // 2-byte sequence
                if ((byte1 & 0xE0) == 0xC0) {
                    byte2 = readContinuationByte();
                    codePoint = ((byte1 & 0x1F) << 6) | byte2;
                    if (codePoint >= 0x80) {
                        return;
                    } else {
                        throw runtime_error("Invalid continuation byte");
                    }
                }

                // 3-byte sequence (may include unpaired surrogates)
                if ((byte1 & 0xF0) == 0xE0) {
                    byte2 = readContinuationByte();
                    byte3 = readContinuationByte();
                    codePoint = ((byte1 & 0x0F) << 12) | (byte2 << 6) | byte3;
                    if (codePoint >= 0x0800) {
                        if (codePoint >= 0xD800 && codePoint <= 0xDFFF) {
                            //throw runtime_error("Lone surrogate U+" + codePoint.toString(16).toUpperCase() + " is not a scalar value");
                            throw runtime_error("Lone surrogate U+xxxxx is not a scalar value");
                        }
                        return;
                    } else {
                        throw runtime_error("Invalid continuation byte");
                    }
                }

                // 4-byte sequence
                if ((byte1 & 0xF8) == 0xF0) {
                    byte2 = readContinuationByte();
                    byte3 = readContinuationByte();
                    byte4 = readContinuationByte();
                    codePoint = ((byte1 & 0x07) << 0x12) | (byte2 << 0x0C) | (byte3 << 0x06) | byte4;
                    if ((codePoint >= 0x010000) && (codePoint <= 0x10FFFF)) {
                        return;
                    }
                }

                throw runtime_error("Invalid UTF-8 detected");
            } catch (...) {
                codePoint = -1;
            }
        }
    public:
        Utf8 (const char* _position) : position (_position), next (_position) {
            readNext ();
        }

        Utf8& operator ++ () {
            position = next;
            readNext ();
            return *this;
        }

        int operator * () {
            return codePoint;
        }
};

