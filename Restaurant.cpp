#include "main.h"

class imp_res : public Restaurant
{
public:
	customer *table = nullptr;	// Doubly circular linked list represents the table, always point to the most recent changed position.
	customer *queue = nullptr;	// Doubly linked list presents the queue, always points to head of the queue.
	customer *recent = nullptr; // Doubly linked list present the recent customers, can be used as queue, always point to the head.
	enum position
	{
		DEFAULT = 0,
		CLOCKWISE,
		ANTICLOCKWISE
	};
	/*
	next is CLOCKWISE
	prev is ANTICLOCKWISE
	*/
	imp_res(){};
	int sizeTable = 0;
	int sizeQueue = 0;

	void
	addRecent(string name, int energy) // add tail
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
		++sizeTable;
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
			temp->next = add;
		}
		++sizeQueue;
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
		// temp is currently table->prev
		if (name == temp->name)
			return;
		temp = queue;
		while (temp != nullptr)
		{
			if (name == temp->name)
				return;
		}
		delete temp;
		if (table == nullptr)
		{
			addTable(name, energy);
			return;
		}
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
				if (abs(abs(energy) - abs(temp->energy)) > absRES)
				{
					absRES = abs(abs(energy) - abs(temp->energy));
					RES = abs(energy) - abs(temp->energy);
					customer *tempTable = table;
					table = temp;
				}
				temp = temp->next;
			}
			// temp is currently table->prev
			if (abs(abs(energy) - abs(temp->energy)) > absRES)
			{
				absRES = abs(abs(energy) - abs(temp->energy));
				RES = abs(energy) - abs(temp->energy);
				customer *tempTable = table;
				table = temp;
			}
			if (RES < 0)
				addTable(name, energy, ANTICLOCKWISE);
			else
				addTable(name, energy, CLOCKWISE);
		}
		else
		{
			if (sizeQueue >= MAXSIZE)
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

	void removeQueue()
	{
		if (queue == nullptr)
		{
			cout << "no queue \n";
			return;
		}
		if (queue->next == nullptr)
		{
			queue = nullptr;
			sizeQueue = 0;
			return;
		}
		customer *temp = queue;
		queue = queue->next;
		queue->prev = nullptr;
		--sizeQueue;
		delete temp;
	}

	void BLUE(int num)
	{
		if (num == 0)
			return;

		if (num >= sizeTable)
		{
			for (int i = 0; i < sizeTable - 1; i++)
				table = table->next;
			delete table;
			table = nullptr;
			sizeTable = 0;
			return;
		}

		for (int i = 0; i < num; i++) // remove recent got in table - delete in table
		{
			customer *remove = findCustomer(recent->name);
			customer *removePrev = remove->prev;
			customer *removeNext = remove->next;
			removePrev->next = removeNext;
			removeNext->prev = removePrev;
			delete remove;
			--sizeTable;
			removeRecent();
		}

		if (sizeQueue == 0)
			return;

		while (sizeTable < MAXSIZE || queue != nullptr)
		{
			RED(queue->name, queue->energy);
			removeQueue();
		}
		cout << "blue " << num << endl;
	}

	void swapCustomer(customer *one, customer *two)
	{ // use for swapping in shell sort or in table
		if (one == two || one->name == two->name)
			return;
		if (one->next == two || two->next == one) // adjacent
		{
			bool head = false;
			if (one->next == two)
			{
				customer *tempOnePrev = one->prev;
				customer *tempTwoNext = two->next;
				if (tempOnePrev == nullptr)
					head = true;
				else
					tempOnePrev->next = two;
				two->prev = tempOnePrev;
				two->next = one;
				if (tempTwoNext != nullptr)
					tempTwoNext->prev = one;
				one->next = tempTwoNext;
				one->prev = two;
				if (head) // only queue has head
					queue = two;
			}
			else
			{
				customer *tempOneNext = one->next;
				customer *tempTwoPrev = two->prev;
				if (tempOneNext != nullptr)
					tempOneNext->prev = two;
				two->next = tempOneNext;
				two->prev = one;
				if (tempTwoPrev == nullptr)
					head = true;
				else
					tempTwoPrev->next = one;
				one->prev = tempTwoPrev;
				one->next = two;
				if (head)
					queue = one;
			}
			return;
		}

		bool head1 = false;
		bool head2 = false;

		customer *tempOnePrev = one->prev;
		customer *tempOneNext = one->next;
		customer *tempTwoPrev = two->prev;
		customer *tempTwoNext = two->next;

		if (tempOnePrev == nullptr)
			head1 = true;
		else
			tempOnePrev->next = two;
		if (tempOneNext != nullptr)
			tempOneNext->prev = two;
		two->prev = tempOnePrev;
		two->next = tempOneNext;

		if (tempTwoPrev == nullptr)
			head2 = true;
		else
			tempTwoPrev->next = one;
		if (tempTwoNext != nullptr)
			tempTwoNext->prev = one;
		one->prev = tempTwoPrev;
		one->next = tempTwoNext;
		if (head1)
			queue = two;
		if (head2)
			queue = one;
	}

	customer *getElementQueue(int index)
	{
		if (index < 0 || index >= sizeQueue)
		{
			cout << "out of index";
			return nullptr;
		}
		if (index == 0)
			return queue;
		customer *temp = queue;
		for (int i = 0; i < index; i++)
		{
			temp = temp->next;
		}
		return temp;
	}

	void PURPLE()
	{
		int N = 0;
		int pos;
		int largestEnergy = 0;
		customer *temp = queue;
		for (int i = 0; i < sizeQueue; i++)
		{
			if (abs(temp->energy) >= largestEnergy)
			{
				largestEnergy = temp->energy;
				pos = i;
			}
			temp = temp->next;
		}
		// Perform shell sort from positon 0 to pos on the queue.

		int gap = (pos + 1) / 2;
		while (gap > 0)
		{
			for (int i = gap; i <= pos; i++)
			{
				for (int j = i; j >= gap && getElementQueue(j - gap)->energy > getElementQueue(j)->energy; j -= gap)
				{
					swapCustomer(getElementQueue(j), getElementQueue(j - gap));
					N++;
				}
			}
			gap /= 2;
		}

		BLUE(N);
		cout << "purple" << endl;
	}

	void REVERSAL()
	{
		// split into two part: positive energy and negative energy, reverse each part.
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