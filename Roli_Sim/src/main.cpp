/*
 * main.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: dylan
 */


#include <iostream>
#include "TRandom3.h"

using namespace std;


int main() {
	TRandom3 Test1(0);
	TRandom3 Test2(0);

	for(int i = 0; i < 50; i++) {
		cout << "Test1: " << Test1.Rndm() << " | Test2: " << Test2.Rndm() << endl;
	}

	return(0);
}

