#ifndef StPicoArrays_h
#define StPicoArrays_h

//_________________
class StPicoArrays {

 public:

  StPicoArrays();

  /// Should be changed to constexpr once ROOT 6 is available at STAR
  enum { NAllPicoArrays = 14};

  /// Names of the TBranches in the TTree/File
  static const char* picoArrayNames[NAllPicoArrays];

  /// Names of the classes, the TClonesArrays are arrays of this type
  static const char* picoArrayTypes[NAllPicoArrays];

  /// Maximum sizes of the TClonesArrays
  static int picoArraySizes[NAllPicoArrays];

  enum TypeIndex { Event=0, Track, EmcTrigger, MtdTrigger,
		   BTowHit, BTofHit, MtdHit, BbcHit, EpdHit, FmsHit,
		   BEmcPidTraits, BTofPidTraits, MtdPidTraits, TrackCovMatrix };

};

#endif
