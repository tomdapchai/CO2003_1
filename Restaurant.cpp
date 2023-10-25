#include "main.h"

class imp_res : public Restaurant
{
public:
	customer *table;	// Doubly circular linked list represents the table, always point to the most recent changed position.
	customer *queue;	// Doubly linked list presents the queue, always points to head of the queue.
	customer *resOrder; // Doubly linked list presents the order of customer arrival to the restaurant
	int sizeTable = 0;
	int sizeQueue = 0;
	int N = 0;
	enum Direction
	{
		DEFAULT = 0,
		CLOCKWISE,
		ANTICLOCKWISE
	};
	/*
	next is CLOCKWISE
	prev is ANTICLOCKWISE
	*/
	imp_res()
	{
		table = nullptr;
		queue = nullptr;
		resOrder = nullptr;
	};

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
	void addTable(string name, int energy, Direction direction = DEFAULT)
	{
		switch (direction)
		{
		case CLOCKWISE:
			if (table->next == table)
			{
				customer *newCus = new customer(name, energy, table, table);
				table->next = newCus;
				table->prev = newCus;
				table = newCus;
			}
			else
			{
				customer *temp = table->next;
				customer *newCus = new customer(name, energy, table, temp);
				temp->prev = newCus;
				table->next = newCus;
				table = newCus;
			}
			break;
		case ANTICLOCKWISE:
			if (table->prev == table)
			{
				customer *newCus = new customer(name, energy, table, table);
				table->next = newCus;
				table->prev = newCus;
				table = newCus;
			}
			else
			{
				customer *temp = table->prev;
				customer *newCus = new customer(name, energy, temp, table);
				table->prev = newCus;
				temp->next = newCus;
				table = newCus;
			}
			break;
		default:
			table = new customer(name, energy, nullptr, nullptr);
			table->next = table;
			table->prev = table;
			break;
		}
		++sizeTable;
	}

	customer *findCustomer(customer *head, string name)
	{
		customer *temp = head;
		if (head == table)
		{
			for (int i = 0; i < sizeTable; i++)
			{
				if (temp->name == name)
					return temp;
				temp = temp->next;
			}
			return nullptr;
		}
		while (temp != nullptr && temp->name != name)
		{
			temp = temp->next;
		}
		return temp;
	}

	void remove(customer *&head, customer *r = nullptr, bool isQueue = false)
	{
		if (head == nullptr)
		{
			return;
		} // ok
		if ((head->next == nullptr && head->prev == nullptr) || (head == table && sizeTable == 1))
		{
			if (head == table)
				sizeTable = 0;
			delete head;
			head = nullptr;
			if (isQueue)
				sizeQueue = 0;
			return;
		}
		if (r == nullptr || (r == head && head != table)) // queue and resOrder remove head
		{
			customer *temp = head;
			head = head->next;
			head->prev = nullptr;
			delete temp;
			if (isQueue)
				--sizeQueue;
		}
		else // remove at other places
		{
			if (head == table) // table
			{
				if (r->energy > 0)
					table = r->next;
				else
					table = r->prev;

				r->prev->next = r->next;
				r->next->prev = r->prev;
				--sizeTable;
			}
			else // queue or resOrder
			{
				if (r->next == nullptr) // r = tail
				{
					r->prev->next = nullptr;
				}
				else
				{
					r->prev->next = r->next;
					r->next->prev = r->prev;
				}
				if (isQueue)
					--sizeQueue;
			}
			delete r;
			r = nullptr;
		}
	}

	void swapCustomer(customer *one, customer *two, customer *head = nullptr)
	{ // use for swapping in shell sort or in table
		if (one == two || (one == nullptr || two == nullptr) || one->name == two->name || (one->next == two && two->next == one))
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

		if (head1 || head2)
			queue = head1 ? two : one;
	}

	customer *getCustomer(customer *head, int index, bool isQueue = false)
	{
		if (head == nullptr)
		{
			return nullptr;
		}
		if (isQueue)
		{
			if (index < 0 || index >= sizeQueue)
			{
				return nullptr;
			}
		}
		customer *temp = head;
		for (int i = 0; i < index; i++)
			temp = temp->next;
		return temp;
	}

	void RED(string name, int energy)
	{
		if (energy == 0 || MAXSIZE == 0)
			return;
		if (table == nullptr)
		{
			addTable(name, energy);
			if (findCustomer(resOrder, name) == nullptr)
				add(resOrder, name, energy);
			return;
		}
		/* customer *temp = resOrder;
		while (temp != nullptr)
		{
			if (name == temp->name && (findCustomer(queue, name) == nullptr))
				return;
			temp = temp->next;
		} */
		customer *temp = table;
		while (1)
		{
			if (name == temp->name)
				return;
			temp = temp->next;
			if (temp == table)
				break;
		}

		if (sizeTable >= MAXSIZE)
		{
			temp = queue;
			while (temp != nullptr)
			{
				if (name == temp->name)
					return;
				temp = temp->next;
			}
		}

		if (sizeTable < MAXSIZE / 2)
		{
			if (energy >= table->energy)
				addTable(name, energy, CLOCKWISE);
			else
				addTable(name, energy, ANTICLOCKWISE);
			if (findCustomer(resOrder, name) == nullptr)
				add(resOrder, name, energy);
		}
		else if ((sizeTable >= MAXSIZE / 2) && sizeTable < MAXSIZE)
		{
			int absRES = 0;
			int RES;
			customer *temp = table;
			while (1)
			{
				if (abs(energy - temp->energy) > absRES)
				{
					absRES = abs(energy - temp->energy);
					RES = energy - temp->energy;
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

			if (findCustomer(resOrder, name) == nullptr)
				add(resOrder, name, energy);
		}
		else
		{
			if (sizeQueue >= MAXSIZE)
				return;
			else
			{
				add(queue, name, energy, true);
				add(resOrder, name, energy);
			}
		}
	}

	void BLUE(int num)
	{
		if (num == 0)
			return;

		if (num >= sizeTable)
		{
			while (table != nullptr)
			{
				remove(resOrder, findCustomer(resOrder, table->name));
				remove(table, table);
			}
			/* for (int i = 0; i < sizeTable - 1; i++)
			{
				customer *temp = table;
				remove(resOrder, findCustomer(resOrder, temp->name));
				table = table->next;
				delete temp;
			}
			delete table; */
			table = nullptr;
			sizeTable = 0;

			while (sizeTable < MAXSIZE && queue != nullptr)
			{
				RED(queue->name, queue->energy);
				// remove(queueOrder, findCustomer(queueOrder, queue->name));
				remove(queue, nullptr, true);
			}
			return;
		}

		for (int i = 0; i < num; i++) // remove recent got in table - delete in table - change table
		{
			/* customer *removePtr = findCustomer(table, recent->name); */
			customer *temp = resOrder;
			while (findCustomer(table, temp->name) == nullptr)
			{
				temp = temp->next;
			}
			customer *removePtr = findCustomer(table, temp->name);
			// locate where table points to
			/* if (removePtr->energy > 0)
				table = removePtr->next;
			else
				table = removePtr->prev; */
			remove(table, removePtr);
			remove(resOrder, temp);
		}

		if (sizeQueue == 0)
			return;

		while (sizeTable < MAXSIZE && sizeQueue > 0)
		{
			RED(queue->name, queue->energy);
			remove(queue, nullptr, true);
		}
	}

	void inssort(int idx, int n, int incr)
	{
		customer *Temp = queue;
		for (int i = 0; i < idx; i++)
			Temp = Temp->next;
		for (int i = incr; i < n; i += incr)
		{
			for (int j = i; j >= incr && abs(getCustomer(Temp, j - incr, true)->energy) <= abs(getCustomer(Temp, j, true)->energy); j -= incr)
			{
				if (abs(getCustomer(Temp, j - incr, true)->energy) < abs(getCustomer(Temp, j, true)->energy)) // swap
				{
					customer *one = getCustomer(Temp, j - incr);
					customer *two = getCustomer(Temp, j);
					bool head = false;
					if (one == Temp)
						head = true;
					swapCustomer(one, two);
					if (head)
						Temp = two;
					N++;
				}
				else // equal value, check which one come first to swap
				{
					int pos1Origin = 0, pos2Origin = 0;

					customer *i = resOrder;
					while (i->name != getCustomer(Temp, j - incr, true)->name)
					{
						pos1Origin++;
						i = i->next;
					}

					i = resOrder;
					while (i->name != getCustomer(Temp, j, true)->name)
					{
						pos2Origin++;
						i = i->next;
					}

					if (pos1Origin > pos2Origin)
					{
						customer *one = getCustomer(Temp, j - incr);
						customer *two = getCustomer(Temp, j);
						bool head = false;
						if (one == Temp)
							head = true;
						swapCustomer(one, two);
						if (head)
							Temp = two;
						N++;
					}
				}
			}
		}
	}
	void shellsort(int n)
	{
		N = 0;
		for (int i = n / 2; i > 2; i /= 2)
			for (int j = 0; j < i; j++)
				inssort(j, n - j, i);
		inssort(0, n, 1);
	}

	void PURPLE()
	{
		if (queue == nullptr || queue->next == nullptr)
			return;
		int pos;
		int largestEnergy = 0;
		customer *temp = queue;
		while (temp != nullptr)
		{
			if (abs(temp->energy) >= largestEnergy)
				largestEnergy = abs(temp->energy);
			temp = temp->next;
		}

		temp = resOrder;
		while (temp != nullptr)
		{
			if (abs(temp->energy) == largestEnergy && findCustomer(queue, temp->name))
			{
				customer *tempTemp = queue;
				int i = 0;
				while (tempTemp != findCustomer(queue, temp->name))
				{
					i++;
					tempTemp = tempTemp->next;
				}
				pos = i;
			}
			temp = temp->next;
		}
		// Perform shell sort from positon 0 to pos on the queue.
		shellsort(pos + 1);

		BLUE(N % MAXSIZE); // leak goes here - definitely, same block as line 328
	}
	void REVERSAL()
	{
		if (sizeTable <= 2)
			return;
		// split into two part: positive energy and negative energy, reverse each part.
		customer *headPos = nullptr;
		customer *headNeg = nullptr;
		customer *temp = table;
		while (1)
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

			if (temp == table)
				break;
		}

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

		while (headPos != nullptr)
			remove(headPos, nullptr);
		while (headNeg != nullptr)
			remove(headNeg, nullptr);
	}

	void UNLIMITED_VOID()
	{
		if (sizeTable < 4)
			return;
		if (sizeTable == 4)
		{
			customer *temp = table;
			int minE = __INT32_MAX__;
			int minP;
			for (int i = 0; i < 4; i++)
			{
				if (temp->energy <= minE)
				{
					minE = temp->energy;
					minP = i;
				}
				temp = temp->next;
			}

			temp = table;
			for (int i = 0; i < minP; i++)
				temp = temp->next;

			for (int i = 0; i < 4; i++)
			{
				temp->print();
				temp = temp->next;
			}
			return;
		}
		// normal case
		customer *temp = table;

		int sum = __INT32_MAX__;
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
			tempTemp = tempTemp->prev;
			int tempLength = 4;
			// now sum = first 4, length = 4, start comparing
			while (tempTemp != temp)
			{
				if ((tempSum < sum) || ((tempSum == sum) && (tempLength >= length)))
				{
					sum = tempSum;
					length = tempLength;
					Name = temp->name;
				}
				++tempLength;
				tempTemp = tempTemp->next;
				tempSum += tempTemp->energy;
			}
			// after the loop we now get the smallest sum with longest length possible, with the Name of customer holding that subsequence
			/*********/
			temp = temp->next;
			if (temp == table)
				break;
		}

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
			temp->print();
			temp = temp->next;
			--length;
		}

		temp = findCustomer(table, Name);
		while (length > 0)
		{
			temp->print();
			temp = temp->next;
			--length;
		}
	}

	void DOMAIN_EXPANSION()
	{
		if (sizeTable + sizeQueue <= 1)
			return;

		int sum = 0;

		customer *temp = resOrder;
		while (temp != nullptr)
		{
			sum += temp->energy;
			temp = temp->next;
		}

		if (queue != nullptr)
		{
			// Print the removed customers
			temp = resOrder;
			while (temp->next != nullptr)
				temp = temp->next;
			// temp: tail of queue, most recent get in the restaurant
			// Start removing in queue
			while (temp != nullptr)
			{
				customer *tempTemp = temp;
				temp = temp->prev;
				if (findCustomer(queue, tempTemp->name) != nullptr)
				{
					if (sum >= 0 ? tempTemp->energy < 0 : tempTemp->energy > 0)
					{
						tempTemp->print();
						remove(queue, findCustomer(queue, tempTemp->name), true);
						remove(resOrder, tempTemp);
					}
				}
			}
		}

		temp = resOrder;
		while (temp->next != nullptr)
			temp = temp->next;
		// temp : tail = most recent get in the table
		// Start removing in table and recent
		while (temp != nullptr)
		{
			customer *tempTemp = temp;
			temp = temp->prev;
			if (findCustomer(table, tempTemp->name) != nullptr)
			{
				if (sum >= 0 ? tempTemp->energy < 0 : tempTemp->energy > 0)
					tempTemp->print();
			}
		}

		temp = resOrder;
		while (temp != nullptr)
		{
			customer *tempTemp = temp;
			temp = temp->next;
			if (findCustomer(table, tempTemp->name) != nullptr)
			{
				if (sum >= 0 ? tempTemp->energy < 0 : tempTemp->energy > 0)
				{
					remove(table, findCustomer(table, tempTemp->name));
					remove(resOrder, tempTemp);
				}
			}
		}

		// Add customer from queue
		while (sizeTable < MAXSIZE && queue != nullptr)
		{
			RED(queue->name, queue->energy);
			remove(queue, nullptr, true);
		}
	}
	void LIGHT(int num)
	{
		if (num != 0)
		{
			if (table == nullptr)
				return;
			if (table->next == table)
			{
				table->print();
				return;
			}
		}
		else
		{
			if (queue == nullptr)
				return;
		}
		if (num > 0)
		{
			customer *temp = table;
			while (1)
			{
				temp->print();
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
				temp->print();
				temp = temp->next;
			}
		}
		else
		{
			customer *temp = table;
			while (1)
			{
				temp->print();
				temp = temp->prev;
				if (temp == table)
					break;
			}
		}
	}
	~imp_res()
	{
		while (table != nullptr)
			remove(table, table);

		while (queue != nullptr)
			remove(queue, nullptr, true);

		while (resOrder != nullptr)
			remove(resOrder, nullptr);
	}
};