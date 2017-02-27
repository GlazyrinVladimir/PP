#include "stdafx.h"
#include <iostream>
#include <omp.h>
#include <random>
#include <sstream>
#include <iostream>

using namespace std;

const float radius = 1000.f;

double GetNumberPi(size_t iterationsNumber)
{
	size_t Nmax = iterationsNumber;
	double Ncirc = 0;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(0.f, radius);

	for (size_t i = 0; i < Nmax; ++i)
	{
		double x = dist(gen);
		double y = dist(gen);
		if (pow(x, 2) + pow(y, 2) <= pow(radius, 2))
		{
			++Ncirc;
		}
	}

	double Pi = (Ncirc / Nmax) * 4;

	return Pi;
}

int main(int argc, char **argv)
{
	int iterationsNumber = atoi(argv[1]);
	std::vector<double> piVector;
#pragma omp parallel num_threads(iterationsNumber)
#pragma omp for schedule(dynamic, 10)
	for (int n = 1; n < 100; n++)
	{
		double pi = GetNumberPi(n);
		std::cout << pi << std::endl;
		piVector.push_back(pi);
	}
	double sum = 0;

#pragma omp for
	for (int i = 0; i < piVector.size(); i++)
	{
		sum += piVector[i];
	}
	std::cout << "sum = " << sum / piVector.size() << std::endl;

	return EXIT_SUCCESS;
}