/*
 * test_class.h
 *
 *  Created on: Sep 9, 2019
 *      Author: Dylan Neff
 */

#ifndef TRACK_H_
#define TRACK_H_


#include <vector>

#include <TObject.h>

using namespace std;


class Track {
public:
	int num;
	Track();
	vector<double> track_size;
	virtual ~Track();
	void clear();
};



#endif /* TRACK_H_ */
