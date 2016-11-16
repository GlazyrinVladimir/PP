#include "stdafx.h"
#include "CThreadInverseMatrix.h"

void  StartParall(CThreadInverseMatrix * matr, size_t num)
{
	matr->ThreadFunc(num);
}

CThreadInverseMatrix::CThreadInverseMatrix(std::vector<std::vector<double>> matrix, std::vector<std::vector<double>> eMatrix)
{
	m_eMatrix = eMatrix;
	m_matrix = matrix;
	DoInverseMatrix();
};

CThreadInverseMatrix::CThreadInverseMatrix(std::vector<std::vector<double>> matrix, std::vector<std::vector<double>> eMatrix, int threadNumber)
{
	m_eMatrix = eMatrix;
	m_matrix = matrix;
	maxThread = threadNumber;
	DoParallInverseMatrix();
};


CThreadInverseMatrix::~CThreadInverseMatrix()
{
};

void CThreadInverseMatrix::WriteInverseMatrix()
{
	std::cout << "inverse matrix" << std::endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			m_eMatrix[i][j] = m_eMatrix[i][j];
			std::cout << m_eMatrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void CThreadInverseMatrix::DoInverseMatrix()
{
	for (int diagonal = 0; diagonal < n; diagonal++)
	{
		if (m_matrix[diagonal][diagonal] != 1)
		{
			for (int k = diagonal + 1; k < n; k++)
			{
				m_matrix[diagonal][k] = m_matrix[diagonal][k] / m_matrix[diagonal][diagonal];
			}
			for (int k = 0; k < n; k++)
			{
				m_eMatrix[diagonal][k] = m_eMatrix[diagonal][k] / m_matrix[diagonal][diagonal];
			}
			m_matrix[diagonal][diagonal] /= m_matrix[diagonal][diagonal];
		}
		for (int i = diagonal + 1; i < n; i++)
		{
			double storage = m_matrix[i][diagonal];

			for (int j = diagonal; j < n; j++)
			{
				m_matrix[i][j] -= m_matrix[diagonal][j] * storage;
			}
			for (int j = 0; j < n; j++)
			{
				m_eMatrix[i][j] -= m_eMatrix[diagonal][j] * storage;
			}
		}
		for (int i = 0; i < diagonal; i++)
		{
			double storage = m_matrix[i][diagonal];

			for (int j = diagonal; j < n; j++)
			{
				m_matrix[i][j] -= m_matrix[diagonal][j] * storage;
			}
			for (int j = 0; j < n; j++)
			{
				m_eMatrix[i][j] -= m_eMatrix[diagonal][j] * storage;
			}
		}
	}
}


void CThreadInverseMatrix::DoParallInverseMatrix()
{
	
	thrs.resize(maxThread);
	size_t currentThread = 0;
	for (int i = 0; i < n; i++)
	{
		thrs[currentThread] = std::thread(StartParall, this, i);
		currentThread++;
		if (currentThread == maxThread)
		{
			for (int j = 0; j < maxThread; j++)
			{
				thrs[j].join();
			}
			currentThread = 0;
		}
	}

	for (int j = 0; j < currentThread; j++)
	{
		thrs[j].join();
	}

}

void CThreadInverseMatrix::ThreadFunc(int number)
{
	int diagonal = number;

	if (m_matrix[diagonal][diagonal] != 1)
	{
		for (int k = diagonal + 1; k < n; k++)
		{
			m_matrix[diagonal][k] = m_matrix[diagonal][k] / m_matrix[diagonal][diagonal];
		}
		for (int k = 0; k < n; k++)
		{
			m_eMatrix[diagonal][k] = m_eMatrix[diagonal][k] / m_matrix[diagonal][diagonal];
		}
		m_matrix[diagonal][diagonal] /= m_matrix[diagonal][diagonal];
	}
	for (int i = diagonal + 1; i < n; i++)
	{
		double storage = m_matrix[i][diagonal];

		for (int j = diagonal; j < n; j++)
		{
			m_matrix[i][j] -= m_matrix[diagonal][j] * storage;
		}
		for (int j = 0; j < n; j++)
		{
			m_eMatrix[i][j] -= m_eMatrix[diagonal][j] * storage;
		}
	}
	for (int i = 0; i < diagonal; i++)
	{
		double storage = m_matrix[i][diagonal];

		for (int j = diagonal; j < n; j++)
		{
			m_matrix[i][j] -= m_matrix[diagonal][j] * storage;
		}
		for (int j = 0; j < n; j++)
		{
			m_eMatrix[i][j] -= m_eMatrix[diagonal][j] * storage;
		}
	}
}