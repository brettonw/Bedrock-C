#include "UNIT_TEST/unit_tester.H"
#include "Json.H"


BOOST_AUTO_TEST_CASE(TestJson1) {
    PtrToJsonArr jsonArray = new JsonArr ();
    jsonArray->add (5);
    PtrToJsonObj jsonObject = new JsonObj ();
    jsonObject->put("xxx", jsonArray);
    Text json = jsonObject->toText ();
    cerr << json << endl;
}

BOOST_AUTO_TEST_CASE(TestJson2) {
    PtrToJsonArr jsonArray = PtrToJsonArr(new JsonArr ())->add (5)->add (true)->add ("hello")->add (1.665)->add (PtrToJsonObj(new JsonObj ())->put ("yyy", 15));
    PtrToJsonObj jsonObject = PtrToJsonObj(new JsonObj ())->put("xxx", jsonArray);
    Text json = jsonObject->toText ();
    cerr << json << endl;
}
