#include "main.h"

class imp_res : public Restaurant
{
public:
	customer *table = nullptr;	// Doubly circular linked list represents the table, always point to the most recent changed position. Or is it?
	customer *queue = nullptr;	// Doubly linked list presents the queue
	customer *recent = nullptr; // Doubly linked list present the recent customers, can be used as stack
	enum position
	{
		DEFAULT = 0,
		CLOCKWISE,
		ANTICLOCKWISE
	};
	imp_res(){};
	int countTable()
	{
		if (table == nullptr)
			return 0;
		if (table->next == nullptr && table->prev == nullptr)
			return 1;
		customer *temp = table;
		int result = 0;
		while (temp->next != table)
		{
			result++;
			temp = temp->next;
		}
		return result;
	}
	int countQueue()
	{
		if (queue == nullptr)
			return 0;
		customer *temp = queue;
		int result = 0;
		while (temp != nullptr)
		{
			result++;
			temp = temp->next;
		}
		return result;
	}

	void addRecent(string name, int energy) // add head
	{
		if (recent == nullptr)
		{
			recent = new customer(name, energy, nullptr, nullptr);
		}
		else
		{
			customer *temp = recent;
			while (temp->next != nullptr)
			{
				temp = temp->next;
			}
			customer *add = new customer(name, energy, temp, nullptr);
		}
	}

	void addTable(string name, int energy, position pos = DEFAULT)
	{
		switch (pos)
		{
		case CLOCKWISE:
			if (table->next == nullptr)
			{
				customer *newCus = new customer(name, energy, table, table);
				table->next = newCus;
				table->prev = newCus;
				customer *tempTable = table;
				table = newCus;
			}
			else
			{
				customer *temp = table->next;
				customer *newCus = new customer(name, energy, table, temp);
				temp->prev = newCus;
				table->next = newCus;
				customer *tempTable = table;
				table = newCus;
			}
			break;
		case ANTICLOCKWISE:
			if (table->prev == nullptr)
			{
				customer *newCus = new customer(name, energy, table, table);
				table->next = newCus;
				table->prev = newCus;
				customer *tempTable = table;
				table = newCus;
			}
			else
			{
				customer *temp = table->prev;
				customer *newCus = new customer(name, energy, temp, table);
				table->prev = newCus;
				temp->next = newCus;
				customer *tempTable = table;
				table = newCus;
			}
			break;
		default:
			table = new customer(name, energy, nullptr, nullptr);
			break;
		}
		addRecent(name, energy);
	}

	void addQueue(string name, int energy) // add tail
	{
		if (queue == nullptr)
		{
			queue = new customer(name, energy, nullptr, nullptr);
		}
		else
		{
			customer *temp = queue;
			while (temp->next != nullptr)
			{
				temp = temp->next;
			}
			customer *add = new customer(name, energy, temp, nullptr);
		}
	}

	void RED(string name, int energy)
	{
		if (energy == 0 || MAXSIZE == 0)
			return;
		customer *temp = table;
		while (temp->next != table)
		{
			if (name == temp->name)
				return;
		}
		if (table == nullptr)
		{
			addTable(name, energy);
			return;
		}
		int sizeTable = countTable();
		if (sizeTable < MAXSIZE / 2)
		{
			if (energy >= table->energy)
				addTable(name, energy, CLOCKWISE);
			else
				addTable(name, energy, ANTICLOCKWISE);
		}
		else if (sizeTable >= MAXSIZE / 2 && sizeTable <= MAXSIZE)
		{
			int absRES = 0;
			int RES;
			customer *temp = table;
			while (temp->next != table)
			{
				if (abs(energy - temp->energy) > absRES)
				{
					absRES = abs(energy - temp->energy);
					RES = energy - temp->energy;
					customer *tempTable = table;
					table = temp;
				}
				temp = temp->next;
			}
			if (RES < 0)
				addTable(name, energy, ANTICLOCKWISE);
			else
				addTable(name, energy, CLOCKWISE);
		}
		else
		{
			if (countQueue() >= MAXSIZE)
				return;
			else
			{
				addQueue(name, energy);
			}
		}
		cout << name << " " << energy << endl;
	}

	customer *findCustomer(string name)
	{
		customer *temp = table;
		while (temp->name != name)
		{
			temp = temp->next;
		}
		return temp;
	}

	void removeRecent()
	{
		if (recent == nullptr)
		{
			cout << "no recent \n";
			return;
		}
		customer *temp = recent;
		recent = recent->next;
		recent->prev = nullptr;
		delete temp;
	}

	void BLUE(int num)
	{
		if (num >= countTable())
		{
			for (int i = 0; i < countTable() - 1; i++)
				table = table->next;
			delete table;
			table = nullptr;
			return;
		}
		for (int i = 0; i < num; i++)
		{
			customer *remove = findCustomer(recent->name);
			customer *removePrev = remove->prev;
			customer *removeNext = remove->next;
			removePrev->next = removeNext;
			removeNext->prev = removePrev;
			delete remove;
			removeRecent();
		}
		cout << "blue " << num << endl;
	}

	void swapCustomer(customer *&one, customer *&two)
	{ // use for swapping in shell sort
		if (one->next == two || two->next == one)
		{
			customer *temp = one;
			one = two;
			two = temp;
			return;
		}

		customer *tempOnePrev = one->prev;
		customer *tempOneNext = one->next;
		customer *tempTwoPrev = two->prev;
		customer *tempTwoNext = two->next;

		if (tempOnePrev != nullptr)
			tempOnePrev->next = two;
		if (tempOneNext != nullptr)
			tempOneNext->prev = two;
		two->prev = tempOnePrev;
		two->next = tempOneNext;

		if (tempTwoPrev != nullptr)
			tempTwoPrev->next = one;
		if (tempTwoNext != nullptr)
			tempTwoNext->prev = one;
		one->prev = tempTwoPrev;
		one->next = tempTwoNext;
	}

	void PURPLE()
	{
		int N = 0;
		int pos;
		int largestEnergy = 0;
		customer *temp = queue;
		for (int i = 0; i < countQueue(); i++)
		{
			if (abs(temp->energy) >= largestEnergy)
			{
				largestEnergy = temp->energy;
				pos = i;
			}
			temp = temp->next;
		}
		// Perform shell sort from positon 0 to pos on the queue.

		BLUE(N);
		cout << "purple" << endl;
	}
	void REVERSAL()
	{
		cout << "reversal" << endl;
	}
	void UNLIMITED_VOID()
	{
		cout << "unlimited_void" << endl;
	}
	void DOMAIN_EXPANSION()
	{
		cout << "domain_expansion" << endl;
	}
	void LIGHT(int num)
	{
		cout << "light " << num << endl;
	}
};