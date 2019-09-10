// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME trackdict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "Track.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *Track_Dictionary();
   static void Track_TClassManip(TClass*);
   static void *new_Track(void *p = 0);
   static void *newArray_Track(Long_t size, void *p);
   static void delete_Track(void *p);
   static void deleteArray_Track(void *p);
   static void destruct_Track(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Track*)
   {
      ::Track *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Track));
      static ::ROOT::TGenericClassInfo 
         instance("Track", "Track.h", 19,
                  typeid(::Track), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Track_Dictionary, isa_proxy, 0,
                  sizeof(::Track) );
      instance.SetNew(&new_Track);
      instance.SetNewArray(&newArray_Track);
      instance.SetDelete(&delete_Track);
      instance.SetDeleteArray(&deleteArray_Track);
      instance.SetDestructor(&destruct_Track);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Track*)
   {
      return GenerateInitInstanceLocal((::Track*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Track*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Track_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Track*)0x0)->GetClass();
      Track_TClassManip(theClass);
   return theClass;
   }

   static void Track_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_Track(void *p) {
      return  p ? new(p) ::Track : new ::Track;
   }
   static void *newArray_Track(Long_t nElements, void *p) {
      return p ? new(p) ::Track[nElements] : new ::Track[nElements];
   }
   // Wrapper around operator delete
   static void delete_Track(void *p) {
      delete ((::Track*)p);
   }
   static void deleteArray_Track(void *p) {
      delete [] ((::Track*)p);
   }
   static void destruct_Track(void *p) {
      typedef ::Track current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Track

namespace {
  void TriggerDictionaryInitialization_trackdict_Impl() {
    static const char* headers[] = {
"Track.h",
0
    };
    static const char* includePaths[] = {
"/home/dylan/Software/root/root-6.14.06/obj/include",
"/home/dylan/git/Research/QGP_Fluctuations/Tree_Reader/src/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "trackdict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$Track.h")))  Track;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "trackdict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "Track.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Track", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("trackdict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_trackdict_Impl, {}, classesHeaders, /*has no C++ module*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_trackdict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_trackdict() {
  TriggerDictionaryInitialization_trackdict_Impl();
}
