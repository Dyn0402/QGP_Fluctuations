/*
 * Tree_Cuts.h
 *
 *  Created on: Jul 28, 2020
 *      Author: Dylan Neff
 */

#ifndef TREECUTS_H_
#define TREECUTS_H_

#include <vector>
#include <map>

#include "DcaxyQAer.h"
#include "PileUpQAer.h"
#include "TreeCutsBase.h"

using namespace std;


class TreeCuts : public TreeCutsBase {
public:
	// Structors
	TreeCuts();
	TreeCuts(int energy);
	TreeCuts(int energy, string particle);
	~TreeCuts();

	// Doers
	void set_values();
	void set_values(int energy);
	void set_values(string particle);
	void set_values(int energy, string particle);
};


#endif /* TREECUTS_H_ */
