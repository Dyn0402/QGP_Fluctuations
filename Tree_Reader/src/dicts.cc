// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dicts

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
#include "Event.h"
#include "Track.h"
#include "Track.h"
#include "Event.h"

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
         instance("Track", "Track.h", 12,
                  typeid(::Track), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Track_Dictionary, isa_proxy, 4,
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
   static TClass *Event_Dictionary();
   static void Event_TClassManip(TClass*);
   static void *new_Event(void *p = 0);
   static void *newArray_Event(Long_t size, void *p);
   static void delete_Event(void *p);
   static void deleteArray_Event(void *p);
   static void destruct_Event(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Event*)
   {
      ::Event *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Event));
      static ::ROOT::TGenericClassInfo 
         instance("Event", "Event.h", 17,
                  typeid(::Event), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Event_Dictionary, isa_proxy, 4,
                  sizeof(::Event) );
      instance.SetNew(&new_Event);
      instance.SetNewArray(&newArray_Event);
      instance.SetDelete(&delete_Event);
      instance.SetDeleteArray(&deleteArray_Event);
      instance.SetDestructor(&destruct_Event);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Event*)
   {
      return GenerateInitInstanceLocal((::Event*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Event*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Event_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Event*)0x0)->GetClass();
      Event_TClassManip(theClass);
   return theClass;
   }

   static void Event_TClassManip(TClass* ){
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

namespace ROOT {
   // Wrappers around operator new
   static void *new_Event(void *p) {
      return  p ? new(p) ::Event : new ::Event;
   }
   static void *newArray_Event(Long_t nElements, void *p) {
      return p ? new(p) ::Event[nElements] : new ::Event[nElements];
   }
   // Wrapper around operator delete
   static void delete_Event(void *p) {
      delete ((::Event*)p);
   }
   static void deleteArray_Event(void *p) {
      delete [] ((::Event*)p);
   }
   static void destruct_Event(void *p) {
      typedef ::Event current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Event

namespace ROOT {
   static TClass *vectorlETrackgR_Dictionary();
   static void vectorlETrackgR_TClassManip(TClass*);
   static void *new_vectorlETrackgR(void *p = 0);
   static void *newArray_vectorlETrackgR(Long_t size, void *p);
   static void delete_vectorlETrackgR(void *p);
   static void deleteArray_vectorlETrackgR(void *p);
   static void destruct_vectorlETrackgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<Track>*)
   {
      vector<Track> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<Track>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<Track>", -2, "vector", 216,
                  typeid(vector<Track>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETrackgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<Track>) );
      instance.SetNew(&new_vectorlETrackgR);
      instance.SetNewArray(&newArray_vectorlETrackgR);
      instance.SetDelete(&delete_vectorlETrackgR);
      instance.SetDeleteArray(&deleteArray_vectorlETrackgR);
      instance.SetDestructor(&destruct_vectorlETrackgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<Track> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<Track>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETrackgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<Track>*)0x0)->GetClass();
      vectorlETrackgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETrackgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETrackgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Track> : new vector<Track>;
   }
   static void *newArray_vectorlETrackgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Track>[nElements] : new vector<Track>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETrackgR(void *p) {
      delete ((vector<Track>*)p);
   }
   static void deleteArray_vectorlETrackgR(void *p) {
      delete [] ((vector<Track>*)p);
   }
   static void destruct_vectorlETrackgR(void *p) {
      typedef vector<Track> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<Track>

namespace {
  void TriggerDictionaryInitialization_dicts_Impl() {
    static const char* headers[] = {
"Event.h",
"Track.h",
0
    };
    static const char* includePaths[] = {
"/home/dylan/Software/root/root-6.14.06/obj/include",
"/home/dylan/git/Research/QGP_Fluctuations/Tree_Reader/src/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "dicts dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$Track.h")))  Track;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class __attribute__((annotate("$clingAutoload$Event.h")))  Event;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "dicts dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "Event.h"
#include "Track.h"
/*
 * LinkDef.h
 *
 *  Created on: Sep 11, 2019
 *      Author: Dylan Neff
 */

#ifndef LINKDEF_H_
#define LINKDEF_H_

#include <vector>
#include "Track.h"
#include "Event.h"

#ifdef __MAKECINT__
#pragma link C++ class Event+;
#pragma link C++ class Track+;
#pragma link C++ class vector<Track>+;
#endif




#endif /* LINKDEF_H_ */

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Event", payloadCode, "@",
"Track", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("dicts",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_dicts_Impl, {}, classesHeaders, /*has no C++ module*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_dicts_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_dicts() {
  TriggerDictionaryInitialization_dicts_Impl();
}
