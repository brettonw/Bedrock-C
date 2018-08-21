#include "Test.h"
#include "Http.h"

TEST_CASE(TestHttpGet) {
    Text result = Http::getText ("https://bedrock.brettonw.com/api?event=ok", "result.json");
    TEST_XY(result, "{\"query\":{\"event\":\"ok\"},\"status\":\"ok\"}\n");
}

