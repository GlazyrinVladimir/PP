#include "Bank.h"


CBank::CBank(PRIMITIVE_TYPE type)
	: m_primitiveType(type)
	, m_totalBalance(0)
	, m_clients(std::vector<CBankClient>())
{
	SelectPrimitive();
}

void CBank::SelectPrimitive()
{
	switch (m_primitiveType)
	{
	case PRIMITIVE_TYPE::CriticalSection:
		InitializeCriticalSection(&m_criticalSection);		//указатель на переменную критическая секция
		break;
	case PRIMITIVE_TYPE::Mutex:
		m_hMutex = CreateMutex(NULL, false, NULL);		//атрибут безопастности флаг начального владельца имя объекта
		//);
		break;
	case PRIMITIVE_TYPE::Semaphore:
		m_hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);	// атрибут доступа инициализированное начальное состояние счетчика максимальное количество обращений имя объекта
		//);
		break;
	case PRIMITIVE_TYPE::Event:
		m_hEvent = CreateEvent(NULL, true, false, NULL);	// атрибут защиты    тип сброса TRUE - ручной начальное    состояние TRUE - сигнальное	   имя обьекта
		//);
		break;
	}
}

CBank::~CBank()
{
	switch (m_primitiveType)
	{
	case PRIMITIVE_TYPE::CriticalSection:
		DeleteCriticalSection(&m_criticalSection);
		break;
	case PRIMITIVE_TYPE::Mutex:
		CloseHandle(&m_hMutex);
		break;
	case PRIMITIVE_TYPE::Semaphore:
		CloseHandle(m_hSemaphore);
		break;
	case PRIMITIVE_TYPE::Event:
		CloseHandle(m_hEvent);
		break;
	}
}

CBankClient* CBank::CreateClient()
{
	unsigned clientId = unsigned(m_clients.size());
	m_clients.push_back(CBankClient(this, clientId));
	return &m_clients.back();
}

void CBank::UpdateClientBalance(CBankClient &client, int value)
{
	int totalBalance = GetTotalBalance();
	std::cout << "Client " << client.GetId() << " initiates reading total balance. Total = " << totalBalance << "." << std::endl;
	
	SomeLongOperations();
	totalBalance += value;

	std::cout
		<< "Client " << client.GetId() << " updates his balance with " << value
		<< " and initiates setting total balance to " << totalBalance
		<< ". Must be: " << GetTotalBalance() + value << "." << std::endl;

	if (totalBalance < 0)
	{
		std::cout << std::endl;
		std::cout << "ERROR operation" << std::endl;
		std::cout << "Total balance = " << GetTotalBalance() << std::endl;
		std::cout << "Balance must not be less zero!!!" << std::endl << std::endl;
		return;
	}
	SetTotalBalance(totalBalance);
}

void CBank::CreateThreads()
{
	for (size_t index = 0; index < m_clients.size(); ++index)
	{
		auto & client = m_clients[index];
		m_threads.push_back(CreateThread(NULL, 0, &client.ThreadFunction, &client, CREATE_SUSPENDED, NULL));
	}
}

void CBank::WaitThreads()
{
	for (auto & thread : m_threads)
	{
		ResumeThread(thread);
	}
	WaitForMultipleObjects(m_threads.size(), m_threads.data(), TRUE, INFINITE);
}

int CBank::GetTotalBalance()
{
	return m_totalBalance;
}


void CBank::SetTotalBalance(int value)
{
	switch (m_primitiveType)
	{
	case PRIMITIVE_TYPE::CriticalSection:
		EnterCriticalSection(&m_criticalSection);// указатель на переменную критическая секция
		m_totalBalance = value;
		LeaveCriticalSection(&m_criticalSection);
		break;
	case PRIMITIVE_TYPE::Mutex:
		WaitForSingleObject(m_hMutex, INFINITE);
		m_totalBalance = value;
		ReleaseMutex(m_hMutex);
		break;
	case PRIMITIVE_TYPE::Semaphore:
		WaitForSingleObject(m_hSemaphore, INFINITE);	//m_hSemaphore -указатель ,time
		m_totalBalance = value;
		ReleaseSemaphore(m_hSemaphore, 1, NULL);	// хенд семафора     на сколько изменять счетчик    предыдущее значение
		//);
		break;
	case PRIMITIVE_TYPE::Event:
		SetEvent(m_hEvent);
		m_totalBalance = value;
		ResetEvent(m_hEvent);
		break;
	default:
		m_totalBalance = value;
		break;
	}
}

void CBank::SomeLongOperations()
{
	Sleep(GetSleepDuration());
}

unsigned int CBank::GetSleepDuration()
{
	return rand() % 100;
}