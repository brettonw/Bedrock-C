#include "Test.h"
#include "Http.h"

TEST_CASE(TestHttpGet) {
    Text result = Http::getText ("https://bedrock.brettonw.com/api?event=ok", new File ("result.json"));
    TEST_EQUALS(result, "{\"query\":{\"event\":\"ok\"},\"status\":\"ok\"}\n");
}

