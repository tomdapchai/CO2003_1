#include "main.h"

class imp_res : public Restaurant
{
public:
	customer *table = nullptr; // Doubly circular linked list represents the table
	customer *queue = nullptr; // Doubly linked list presents the queue
	imp_res(){};
	int customerAmount(customer *a) // use to calculate both table amount and queue amount
	{
		if (a == nullptr)
			return 0;
		if (a->next == nullptr && a->prev == nullptr)
			return 1;
		customer *temp = a;
		int result = 1;
		temp = temp->next;
		while (temp->name != a->name)
		{
			result++;
			temp = temp->next;
		}
		return result;
	}

	void addCustomer(customer *list, customer *need, int position = 0)
	{ // use to add customer in table or queue
	}

	void RED(string name, int energy)
	{
		if (energy == 0)
			return;
		if (table == nullptr)
		{
			table = new customer(name, energy, nullptr, nullptr);
		}
		int size = customerAmount(table);
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