#pragma once
#include <glm/glm.hpp>
#include <cmath>
#include <stdlib.h>
#include <time.h>
class Noise {
public:
private:
	void Shuffle(int* tab, int length) {
		srand(time(NULL));
		for (int i = length - 1; i > 0; i--) {
			int index = round(rand() * (i - 1));
			int temp = tab[i];
			tab[i] = tab[index];
			tab[index] = temp;
		}
	}

	int* MakePermutation() {
		int P[256];
		for (int i = 0; i < 256; i++) {
			P[i] = i;
		}
		Shuffle(P, 256);
		return P;
	}

	glm::vec2 GetConstantVector(int v) {
	}
};
