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

	void add(customer *&head, string name, int energy, bool isQueue = false)
	{ // add tail
		if (head == nullptr)
		{
			head = new customer(name, energy, nullptr, nullptr);
		}
		else
		{
			customer *temp = head;
			while (temp->next != nullptr)
			{
				temp = temp->next;
			}
			customer *add = new customer(name, energy, temp, nullptr);
			temp->next = add;
		}
		if (isQueue)
			++sizeQueue;
	}

	/* void addRecent(string name, int energy) // add tail
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
	} */

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
		add(recent, name, energy);
		++sizeTable;
	}

	/* void addQueue(string name, int energy) // add tail
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
	} */

	void RED(string name, int energy)
	{
		if (energy == 0 || MAXSIZE == 0)
			return;
		customer *temp = table;
		while (1)
		{
			if (name == temp->name)
				return;
			temp = temp->next;
			if (temp == table)
				break;
		}
		// temp is currently table->prev
		temp = queue;
		while (temp != nullptr)
		{
			if (name == temp->name)
				return;
			temp = temp->next;
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
			while (1)
			{
				if (abs(abs(energy) - abs(temp->energy)) > absRES)
				{
					absRES = abs(abs(energy) - abs(temp->energy));
					RES = abs(energy) - abs(temp->energy);
					customer *tempTable = table;
					table = temp;
				}
				temp = temp->next;
				if (temp == table)
					break;
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
				add(queue, name, energy, true);
			}
		}
		cout << name << " " << energy << endl;
	}

	customer *findCustomer(customer *head, string name)
	{
		customer *temp = head;
		while (temp->name != name)
		{
			temp = temp->next;
		}
		return temp;
	}

	void remove(customer *&head, customer *r = nullptr, bool isQueue = false)
	{
		if (head->next == nullptr && head->prev == nullptr)
		{
			delete head;
			return;
		}
		if (r == nullptr || (r == queue || r == recent)) // queue and recent use, remove head
		{
			if (head == nullptr)
			{
				cout << "empty \n";
				return;
			}
			if (isQueue)
			{
				if (head->next == nullptr)
				{
					head = nullptr;
					sizeQueue = 0;
					return;
				}
			}
			customer *temp = head;
			head = head->next;
			head->prev = nullptr;
			temp->next = nullptr;
			delete temp;
			if (isQueue)
				--sizeQueue;
		}
		else // remove other places
		{
			if (head == table) // table
			{
				if (r->energy > 0)
					head = r->next;
				else
					head = r->prev;

				r->prev->next = r->next;
				r->next->prev = r->prev;

				--sizeTable;
			}
			else // queue or recent
			{
				if (r->next == nullptr)
				{
					r->prev->next = nullptr;
					r = r->next;
				}
				else
				{
					r->prev->next = r->next;
					r->next->prev = r->prev;
				}
				if (isQueue)
					--sizeQueue;
			}
		}
	}

	/* void removeRecent()
	{ // remove head
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
	} */

	void BLUE(int num)
	{
		if (num == 0)
			return;

		if (num >= sizeTable)
		{
			for (int i = 0; i < sizeTable - 1; i++)
			{
				customer *temp = table;
				table = table->next;
				delete temp;
			}
			delete table;
			table = nullptr;
			sizeTable = 0;
			return;
		}

		for (int i = 0; i < num; i++) // remove recent got in table - delete in table - change table
		{
			customer *removePtr = findCustomer(table, recent->name);
			// locate where table points to
			if (removePtr->energy > 0)
				table = removePtr->next;
			else
				table = removePtr->prev;

			removePtr->prev->next = removePtr->next;
			removePtr->next->prev = removePtr->prev;

			removePtr->prev = nullptr;
			removePtr->next = nullptr;
			delete removePtr;

			--sizeTable;
			remove(recent);
		}

		if (sizeQueue == 0)
			return;

		while (sizeTable < MAXSIZE || queue != nullptr)
		{
			RED(queue->name, queue->energy);
			remove(queue, nullptr, true);
		}
		cout << "blue " << num << endl;
	}

	void swapCustomer(customer *one, customer *two) // later
	{												// use for swapping in shell sort or in table
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

	customer *getCustomer(customer *head, int index, bool isQueue = false)
	{
		//
		if (isQueue)
		{
			if (index < 0 || index >= sizeQueue)
			{
				cout << "out of index";
				return nullptr;
			}
		}
		customer *temp = head;
		for (int i = 0; i < index; i++)
			temp = temp->next;
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
				for (int j = i; j >= gap && getCustomer(queue, j - gap, true)->energy > getCustomer(queue, j, true)->energy; j -= gap)
				{
					swapCustomer(getCustomer(queue, j, true), getCustomer(queue, j - gap, true));
					N++;
				}
			}
			gap /= 2;
		}

		BLUE(N % MAXSIZE);
		cout << "purple" << endl;
	}

	void REVERSAL()
	{
		// split into two part: positive energy and negative energy, reverse each part.
		customer *headPos = nullptr;
		customer *headNeg = nullptr;
		customer *temp = table;
		while (temp->prev != table)
		{
			if (temp->energy > 0)
			{
				add(headPos, temp->name, temp->energy);
			}
			else
			{
				add(headNeg, temp->name, temp->energy);
			}
			temp = temp->prev;
		}
		if (temp->energy > 0)
		{
			add(headPos, temp->name, temp->energy);
		}
		else
		{
			add(headNeg, temp->name, temp->energy);
		}

		temp->next = nullptr;
		temp->prev = nullptr;
		delete temp;

		int sizePos = 0, sizeNeg = 0;
		customer *count = headPos;
		while (count != nullptr)
		{
			sizePos++;
			count = count->next;
		}

		count = headNeg;
		while (count != nullptr)
		{
			sizeNeg++;
			count = count->next;
		}
		delete count;

		// swap positive energy
		for (int i = 0; i < sizePos / 2; i++)
		{
			swapCustomer(findCustomer(table, getCustomer(headPos, i)->name), findCustomer(table, getCustomer(headPos, sizePos - 1 - i)->name));
		}

		// swap negative energy
		for (int i = 0; i < sizeNeg / 2; i++)
		{
			swapCustomer(findCustomer(table, getCustomer(headNeg, i)->name), findCustomer(table, getCustomer(headNeg, sizeNeg - 1 - i)->name));
		}
		// done
		/* headPos->next = nullptr;
		headPos->prev = nullptr;
		headNeg->next = nullptr;
		headNeg->prev = nullptr; */
		// this is not the way to delte headPos and headNeg, have to delete entire list
		delete headPos;
		delete headNeg;
		cout << "reversal" << endl;
	}

	void UNLIMITED_VOID()
	{
		if (sizeTable < 4)
			return;
		if (sizeTable == 4)
		{
			customer *temp = table;
			while (1)
			{
				cout << temp->name << "-" << temp->energy << endl;
				temp = temp->next;
				if (temp == table)
					break;
			}
			return;
		}
		// normal case
		customer *temp = table;

		int sum = 0;
		int length = 0;
		string Name;
		while (1)
		{
			customer *tempTemp = temp;
			int tempSum = 0;
			for (int i = 0; i < 4; i++)
			{
				tempSum += tempTemp->energy;
				tempTemp = tempTemp->next;
			}
			int tempLength = 4;
			// now sum = first 4, length = 4, start comparing
			while (tempTemp != temp)
			{
				if (tempSum < sum || (tempSum == sum && tempLength >= length))
				{
					sum = tempSum;
					length = tempLength;
					Name = temp->name;
				}
				++tempLength;
				tempSum += tempTemp->energy;

				tempTemp = tempTemp->next;
			}
			/* tempTemp->next = nullptr;
			tempTemp->prev = nullptr;
			delete tempTemp; */
			// after the loop we now get the smallest sum with longest length possible, with the Name of customer holding that subsequence
			/*********/
			temp = temp->next;
			if (temp == table)
				break;
		}

		// print
		temp = findCustomer(table, Name);
		string minName = Name;
		int minEnergy = __INT32_MAX__;
		int minPos;
		for (int i = 0; i < length; i++)
		{
			if (temp->energy < minEnergy) // take the first minimum value
			{
				minEnergy = temp->energy;
				minName = temp->name;
				minPos = i;
			}
			temp = temp->next;
		}

		temp = findCustomer(table, minName);
		while (length > minPos)
		{
			cout << temp->name << "-" << temp->energy << endl;
			temp = temp->next;
			--length;
		}
		temp = findCustomer(table, Name);
		while (length > 0)
		{
			cout << temp->name << "-" << temp->energy << endl;
			temp = temp->next;
			--length;
		}

		/* temp->next = nullptr;
		temp->prev = nullptr;
		delete temp; */

		// Add customer from queue
		while (sizeTable < MAXSIZE || queue != nullptr)
		{
			RED(queue->name, queue->energy);
			remove(queue, nullptr, true);
		}

		cout << "unlimited_void" << endl;
	}

	void DOMAIN_EXPANSION()
	{
		int ePos = 0;
		int eNeg = 0;

		customer *temp = table;
		while (1)
		{
			if (temp->energy > 0)
				ePos += temp->energy;
			else
				eNeg += abs(temp->energy);
			temp = temp->next;
			if (temp == table)
				break;
		}
		temp = queue;
		while (temp != nullptr)
		{
			if (temp->energy > 0)
				ePos += temp->energy;
			else
				eNeg += abs(temp->energy);
			temp = temp->next;
		}
		// Print the removed customers
		temp = recent;
		while (temp->next != nullptr)
			temp = temp->next;
		// temp : tail = most recent
		while (temp != nullptr)
		{
			if (ePos >= eNeg ? temp->energy < 0 : temp->energy > 0)
				cout << temp->name << "-" << temp->energy << endl;
			temp = temp->prev;
		}
		// Start removing
		temp = table;
		while (1)
		{
			if (ePos >= eNeg ? temp->energy < 0 : temp->energy > 0)
			{
				remove(recent, findCustomer(recent, temp->name));
				remove(table, temp);
			}

			temp = temp->next;
			if (temp == table)
			{
				temp = nullptr;
				delete temp;
				break;
			}
		}
		temp = queue;
		while (temp != nullptr)
		{
			if (ePos >= eNeg ? temp->energy < 0 : temp->energy > 0)
				remove(queue, temp, true);
			temp = temp->next;
		}
		delete temp;

		cout << "domain_expansion" << endl;
	}

	void LIGHT(int num)
	{
		if (num > 0)
		{
			customer *temp = table;
			while (1)
			{
				cout << temp->name << "-" << temp->energy << endl;
				temp = temp->next;
				if (temp == table)
					break;
			}
		}
		else if (num == 0)
		{
			customer *temp = queue;
			while (temp != nullptr)
			{
				cout << temp->name << "-" << temp->energy << endl;
				temp = temp->next;
			}
		}
		else
		{
			customer *temp = table;
			while (temp->prev != table)
			{
				cout << temp->name << "-" << temp->energy << endl;
				temp = temp->prev;
			}
			cout << temp->name << "-" << temp->energy << endl;
		}
		cout << "light " << num << endl;
	}
	~imp_res()
	{
		for (int i = 0; i < sizeTable; i++)
		{
			customer *temp = table;
			table = table->next;
			delete temp;
		}
		while (queue != nullptr)
		{
			customer *temp = queue;
			queue = queue->next;
			delete temp;
		}
		while (recent != nullptr)
		{
			customer *temp = recent;
			recent = recent->next;
			delete temp;
		}
	}
};