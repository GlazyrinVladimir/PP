#include "stdafx.h"
#include "Constants.h"

class CMatrix
{
public:
	CMatrix();
	~CMatrix();
	void WriteMatrix();
	std::vector<std::vector<double>> GetMatrix();
	std::vector<std::vector<double>> GetEMatrix();
private:
	void FillMatrix();
	bool GetDeterm();
	std::vector<std::vector<double>> m_matrix;
	std::vector<std::vector<double>> m_eMatrix;
};