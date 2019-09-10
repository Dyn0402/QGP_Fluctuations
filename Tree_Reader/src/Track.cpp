/*
 * test_class.cpp
 *
 *  Created on: Sep 9, 2019
 *      Author: Dylan Neff
 */


#include <TObject.h>
#include "Track.h"


Track::Track() {
	num = 5;
}

Track::~Track() {}

void Track::clear() {
	track_size.clear();
	num = 5;
}
