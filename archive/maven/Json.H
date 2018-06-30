// XXX TODO - the output is not "pretty", formatted, or otherwise made especially
// XXX TODO   human readable, though we could certainly do that.

#ifndef     _TEXT_CONTAINER_H_
#include    "TextContainer.H"
#endif //   _TEXT_CONTAINER_H_

#ifndef     _JSON_H_
#define     _JSON_H_

MAKE_PTR_TO(JsonThing) {
    public:
        virtual ~JsonThing () {}

        typedef enum {
            STR_TYPE,
            INT_TYPE,
            FLT_TYPE,
            BLN_TYPE,
            ARR_TYPE,
            OBJ_TYPE
        } Type;

        virtual Type getType () const = 0;
        virtual Text toText () const = 0;

    protected:
        static Text enclose (const Text& value, const Text& beginEnclosure, const Text& endEnclosure) {
            return Text () << beginEnclosure << value << endEnclosure;
        }

        static Text enclose (const Text& value, const Text& enclosure) {
            return enclose (value, enclosure, enclosure);
        }

        static Text enquote (const Text& value) {
            return enclose (value, "\"");
        }
};

class JsonStr : public JsonThing {
    protected:
        Text value;

    public:
        JsonStr (const Text& _value) : value (_value) {}
        virtual ~JsonStr () { }
        virtual JsonThing::Type getType () const { return JsonThing::STR_TYPE; }
        virtual Text toText () const { return enquote (value); }
};

class JsonInt : public JsonThing {
    protected:
        int value;

    public:
        JsonInt (int _value) : value (_value) {}
        virtual ~JsonInt () {}
        virtual JsonThing::Type getType () const { return JsonThing::INT_TYPE; }
        virtual Text toText () const { return Text () << value; }
};

class JsonFlt : public JsonThing {
    protected:
        double value;

    public:
        JsonFlt (double _value) : value (_value) {}
        virtual ~JsonFlt () {}
        virtual JsonThing::Type getType () const { return JsonThing::FLT_TYPE; }
        virtual Text toText () const { return Text () << value; }
};

class JsonBln : public JsonThing {
    protected:
        bool value;

    public:
        JsonBln (bool _value) : value (_value) {}
        virtual ~JsonBln () {}
        virtual JsonThing::Type getType () const { return JsonThing::BLN_TYPE; }
        virtual Text toText () const { return Text () << value; }
};

MAKE_PTR_TO_SUB(JsonArr, JsonThing) {
    protected:
        vector<PtrToJsonThing> value;
        Text beginEnclosure;
        Text endEnclosure;
        Text separator;

    public:
        JsonArr () : beginEnclosure ("["), endEnclosure ("]"), separator (",") {}
        JsonArr (const Text& _begin, const Text& _end, const Text& _separator) : beginEnclosure (_begin), endEnclosure (_end), separator (_separator) {}
        virtual ~JsonArr () {}
        virtual JsonThing::Type getType () const { return JsonThing::ARR_TYPE; }

        virtual Text toText () const {
            Text prepend = "";
            Text out;
            for (vector<PtrToJsonThing>::const_iterator it = value.begin (); it != value.end (); ++it) {
                out << prepend << (*it)->toText ();
                prepend = separator;
            }
            return enclose (out, beginEnclosure, endEnclosure);
        }

        template<typename JsonThingSubtype>
        PtrToJsonArr add (PtrTo<JsonThingSubtype> jsonThing) {
            value.push_back (ptr_upcast<JsonThing>(jsonThing));
            return this;
        }

        PtrToJsonArr add (JsonThing* jsonThing) {
            value.push_back (jsonThing);
            return this;
        }

        PtrToJsonArr add(const char* thing) {
            return add (new JsonStr (thing));
        }

        PtrToJsonArr add(int thing) {
            return add (new JsonInt (thing));
        }

        PtrToJsonArr add(double thing) {
            return add (new JsonFlt (thing));
        }

        PtrToJsonArr add(bool thing) {
            return add (new JsonBln (thing));
        }

        uint size () {
            return value.size();
        }

        PtrToJsonThing get (uint index) {
            return (index < value.size ()) ? value[index] : (JsonThing*)0;
        }

        const PtrToJsonThing get (uint index) const {
            return (index < value.size ()) ? value[index] : (JsonThing*)0;
        }
};

MAKE_PTR_TO_SUB(JsonObj, JsonThing) {
    protected:
        TextMap<PtrToJsonThing> value;

    public:
        JsonObj () {}
        virtual ~JsonObj () {}
        virtual JsonThing::Type getType () const { return JsonThing::OBJ_TYPE; }
        virtual Text toText () const {
            const char* prepend = "";
            Text out;
            for (TextMap<PtrToJsonThing>::const_iterator it = value.begin (); it != value.end (); ++it) {
                out << prepend << enquote (it->first) << ":" << it->second->toText ();
                prepend = ",";
            }
            return enclose (out, "{", "}");
        }

        template<typename JsonThingSubtype>
        PtrToJsonObj put (const char* name, PtrTo<JsonThingSubtype> jsonThing) {
            value.set(name, ptr_upcast<JsonThing>(jsonThing));
            return this;
        }

        PtrToJsonObj put (const char* name, JsonThing* jsonThing) {
            value.set(name, jsonThing);
            return this;
        }

        PtrToJsonObj put (const char* name, const char* thing) {
            return put (name, new JsonStr (thing));
        }

        PtrToJsonObj put (const char* name, int thing) {
            return put (name, new JsonInt (thing));
        }

        PtrToJsonObj put (const char* name, double thing) {
            return put (name, new JsonFlt (thing));
        }

        PtrToJsonObj put (const char* name, bool thing) {
            return put (name, new JsonBln (thing));
        }

        PtrToJsonThing get (const char* name) {
            PtrToJsonThing* handle = value.get (name);
            return (handle) ? *handle : (JsonThing*)0;
        }

        const PtrToJsonThing get (const char* name) const {
            const PtrToJsonThing* handle = value.get (name);
            return (handle) ? *handle : (JsonThing*)0;
        }
};

#endif  //  _JSON_H_
