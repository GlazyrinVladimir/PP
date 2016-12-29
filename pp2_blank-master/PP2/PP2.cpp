#include "stdafx.h"
#include "Bank.h"
#include "BankClient.h"

void PrintHelp()
{
	std::cout << std::endl << "use program.exe amountOfClients NumberOftypePrimitive Help" << std::endl;
	std::cout << "Primitive Types" << std::endl;
	std::cout << "1 - critical section" << std::endl;
	std::cout << "2 - mutex" << std::endl;
	std::cout << "3 - semaphore" << std::endl;
	std::cout << "4 - event" << std::endl;
}

bool wasHelpParametr(int argc, char * argv[])
{
	std::string callHelp = "/?";
	if (argc == 4)
	{
		if (argv[3] == callHelp)
		{
			return true;
		}
	}
	return false;
}

bool isRightPrimitive(char * argv[], PRIMITIVE_TYPE &type)
{
	int value = atoi(argv[2]);
	switch (value)
	{
	case 1:
		type = PRIMITIVE_TYPE::CriticalSection;
		break;
	case 2:
		type = PRIMITIVE_TYPE::Mutex;
		break;
	case 3:
		type = PRIMITIVE_TYPE::Semaphore;
		break;
	case 4:
		type = PRIMITIVE_TYPE::Event;
		break;
	default:
		return false;
	}	
	return true;
}

int main(int argc, char * argv[])
{
	if (argc != 3 && argc != 4)
	{
		std::cout << "Incorrect amount arguments!";
		PrintHelp();
		return 1;
	}
	if (wasHelpParametr(argc, argv))
	{
		PrintHelp();
	}
	size_t numberOfClients = size_t(atoi(argv[1]));
	PRIMITIVE_TYPE type;
	if (!isRightPrimitive(argv, type))
	{
		std::cout << "Incorrect second argument!";
		PrintHelp();
		return 1;
	}
	CBank bank(type);
	for (size_t index = 0; index < numberOfClients; ++index)
	{
		bank.CreateClient();
	}
	bank.CreateThreads();	
	bank.WaitThreads();

    return 0;
}
