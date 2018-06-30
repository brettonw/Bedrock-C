#ifndef     _TYPES_H_
#include    "Types.H"
#endif //   _TYPES_H_

#ifndef     _REFERENCE_COUNTED_OBJECT_H_
#define     _REFERENCE_COUNTED_OBJECT_H_

class ReferenceCountedObject
{
    public:
        ReferenceCountedObject (void){
            referenceCount = 0;
            #ifndef NDEBUG
            debugId = ++nextDebugId;
            #endif
        }

        ReferenceCountedObject (const ReferenceCountedObject& object){
            referenceCount = 0;
            #ifndef NDEBUG
            debugId = ++nextDebugId;
            #endif
        }

virtual ~ReferenceCountedObject (void) {
            ASSERT_CALL(referenceCount == 0);
        }

        void addRef (void){
            referenceCount++;
        }

        uint removeRef (void){
            return --referenceCount;
        }

        uint getReferenceCount (void) const{
            return referenceCount;
        }

    private:
        uint referenceCount;
        
        #ifndef NDEBUG
        uint debugId;
static  uint nextDebugId;
        #endif
};
        
#endif  //  _REFERENCE_COUNTED_OBJECT_H_
