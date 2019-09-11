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
