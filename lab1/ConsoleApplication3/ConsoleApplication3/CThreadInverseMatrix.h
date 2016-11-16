#include "stdafx.h"
#include "Constants.h"

class CThreadInverseMatrix
{
public:
	CThreadInverseMatrix(std::vector<std::vector<double>> matrix, std::vector<std::vector<double>> eMatrix);
	CThreadInverseMatrix(std::vector<std::vector<double>> matrix, std::vector<std::vector<double>> eMatrix, int threadNumber);
	~CThreadInverseMatrix();
	void DoInverseMatrix();
	void DoParallInverseMatrix();
	void ThreadFunc(int number);
private:
	void WriteInverseMatrix();
	std::vector<std::vector<double>> m_matrix;
	std::vector<std::vector<double>> m_eMatrix;
	int maxThread;
	std::vector<std::thread> thrs;
};