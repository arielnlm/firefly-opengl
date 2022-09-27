#ifndef H_INSERTION_SORT
#define H_INSERTION_SORT

#pragma once
#include "Particle.h"
#include <vector>

/*
	Sort algorithm used to sort particles in order in which they should render on screen.
*/
class InsertionSort {
public:
	static void sortHighToLow(std::vector<Particle*> &list) {
		for (int i = 1; i < list.size(); i++) {
			Particle* item = list[i];
			if (item->getDistance() > list[i - 1]->getDistance()) {
				sortUpHighToLow(list, i);
			}
		}
	}

	static void sortUpHighToLow(std::vector<Particle*> &list, int i) {
		Particle* item = list[i];
		int attemptPos = i - 1;
		while (attemptPos != 0 && list[attemptPos - 1]->getDistance() < item->getDistance()) {
			attemptPos--;
		}

		list.erase(list.begin() + i);
		list.insert(list.begin() + attemptPos, item);
	}

};

#endif