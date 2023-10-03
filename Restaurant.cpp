#include "main.h"

class imp_res : public Restaurant
{
public:
	customer *table = nullptr; // Doubly circular linked list represents the table
	customer *queue = nullptr; // Doubly linked list presents the queue
	enum position
	{
		CLOCKWISE = 0,
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
		int result = 1;
		temp = temp->next;
		while (temp->name != table->name)
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

	void addTable(customer *current, customer *add, position pos)
	{
		switch (pos)
		{
		case CLOCKWISE:
			if (current->next == nullptr) {
				current->next = add;
				add->prev = current;
				add->next = nullptr;
			}
			else {
				customer *temp = current->next;
				current->next = add;
				add->prev = current;
				add->next = temp;
				temp->prev = add;
			}
			break;
		case ANTICLOCKWISE:
			if (current->prev == nullptr) {
				current->prev = add;
				add->prev = nullptr;
				add->next = current;
			}
			else {
				customer *temp = current->prev;
				current->prev = add;
				add->next = current;
				add->prev = temp;
				temp->next = add;
			}
			break;
		default:
			break;
		}
	}

	void addQueue(customer *add) // add tail
	{

	}

	void RED(string name, int energy)
	{
		if (energy == 0)
			return;
		if (table == nullptr)
		{
			table = new customer(name, energy, nullptr, nullptr);
			return;
		}
		int size = countTable();
		if (size < MAXSIZE / 2)
		{
		}
		else if (size >= MAXSIZE / 2 && size <= MAXSIZE)
		{
		}
		else
		{
		}
		cout << name << " " << energy << endl;
	}

	void removeCustomer()
	{ // only remove at table
	}

	void BLUE(int num)
	{
		cout << "blue " << num << endl;
	}
	void PURPLE()
	{
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