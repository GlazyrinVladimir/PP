// ConsoleApplication3.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "matrix.h"
#include "CThreadInverseMatrix.h"


int main()
{
	srand(time(0));
	CMatrix matrix;
	size_t threads;
	std::cout << "Enter number of threads : ";
	std::cin >> threads;
	double matrixCreateTime = clock() / 1000.0;
//	CThreadInverseMatrix invmatrix(matrix.GetMatrix(), matrix.GetEMatrix());
	double inverseMatrixCreateTime = clock() / 1000.0;
//	std::cout << "One Thread Metod = " << inverseMatrixCreateTime - matrixCreateTime << std::endl;
	CThreadInverseMatrix invParallmatrix(matrix.GetMatrix(), matrix.GetEMatrix(), threads);
	double inverseParallMatrixCreateTime = clock() / 1000.0;
	std::cout << "MultiThread Metod = " << inverseParallMatrixCreateTime - inverseMatrixCreateTime << std::endl;
	return 0;
}

