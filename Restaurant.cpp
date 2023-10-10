#include "main.h"

class imp_res : public Restaurant
{
public:
	customer *table = nullptr;		// Doubly circular linked list represents the table, always point to the most recent changed position.
	customer *queue = nullptr;		// Doubly linked list presents the queue, always points to head of the queue.
	customer *recent = nullptr;		// Doubly linked list present the recent customers, can be used as queue, always point to the head.
	customer *queueOrder = nullptr; // Store the order of customers in queue
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

	void addTable(string name, int energy, Direction direction = DEFAULT)
	{
		switch (direction)
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

	void RED(string name, int energy)
	{
		cerr << "RED check \n";
		if (energy == 0 || MAXSIZE == 0)
			return;
		if (table == nullptr)
		{
			addTable(name, energy);
			return;
		}
		customer *temp = table;
		while (1)
		{
			if (name == temp->name)
				return;
			temp = temp->next;
			if (temp == table || temp == nullptr)
				break;
		}
		// temp is currently table->prev or temp == nullptr
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
				add(queueOrder, name, energy);
			}
		}
		// print table
		cerr << "Table\n";
		temp = table;
		while (1)
		{
			cerr << temp->name << " " << temp->energy << endl;
			temp = temp->next;
			if (temp == table)
				break;
		}
		// print queue
		cerr << "Queue\n";
		temp = queue;
		while (temp != nullptr)
		{
			cerr << temp->name << " " << temp->energy << endl;
			temp = temp->next;
		}
		// print recent
		cerr << "Recent\n";
		temp = recent;
		while (temp != nullptr)
		{
			cerr << temp->name << " " << temp->energy << endl;
			temp = temp->next;
		}
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
		if (head == nullptr)
		{
			delete head;
			return;
		}
		if (head->next == nullptr && head->prev == nullptr)
		{
			customer *temp = head;
			head = head->next;
			delete temp;
			delete head;
			if (isQueue)
				sizeQueue = 0;
			return;
		}
		if (r == nullptr || (r == queue || r == recent || r == queueOrder)) // queue and recent and queueOrder use, remove head
		{
			if (head == nullptr)
			{
				cerr << "empty \n";
				return;
			}
			customer *temp = head;
			head = head->next;
			head->prev = nullptr;
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
			else // queue or recent or queueOrder
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
			delete r;
		}
	}

	void BLUE(int num)
	{
		cerr << "BLUE check \n";
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
			while (recent != nullptr)
			{
				customer *temp = recent;
				recent = recent->next;
				delete temp;
			}
			delete recent;
			recent = nullptr;

			while (sizeTable < MAXSIZE && queue != nullptr)
			{
				RED(queue->name, queue->energy);
				remove(queueOrder, findCustomer(queueOrder, queue->name));
				remove(queue, nullptr, true);
			}
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
			remove(table, removePtr);
			remove(recent);
		}

		customer *temp = table;
		while (1)
		{
			cerr << temp->name << " " << temp->energy << endl;
			temp = temp->next;
			if (temp == table)
				break;
		}

		if (sizeQueue == 0)
			return;

		while (sizeTable < MAXSIZE && queue != nullptr)
		{
			RED(queue->name, queue->energy);
			remove(queueOrder, findCustomer(queueOrder, queue->name));
			remove(queue, nullptr, true);
		}

		cerr << "Table\n";
		temp = table;
		while (1)
		{
			cerr << temp->name << " " << temp->energy << endl;
			temp = temp->next;
			if (temp == table)
				break;
		}
		// print queue
		cerr << "Queue\n";
		temp = queue;
		while (temp != nullptr)
		{
			cerr << temp->name << " " << temp->energy << endl;
			temp = temp->next;
		}
		// print recent
		cerr << "Recent\n";
		temp = recent;
		while (temp != nullptr)
		{
			cerr << temp->name << " " << temp->energy << endl;
			temp = temp->next;
		}
	}

	void swapCustomer(customer *one, customer *two)
	{ // use for swapping in shell sort or in table
		if (one == two || (one == nullptr || two == nullptr) || one->name == two->name)
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
		if (head == nullptr)
		{
			cerr << "no head \n";
			return nullptr;
		}
		if (isQueue)
		{
			if (index < 0 || index >= sizeQueue)
			{
				cerr << index << " out of index";
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
		cerr << "========================================================= \n";
		cerr << "PURPLE check \n";
		if (queue == nullptr || queue->next == nullptr)
			return;
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
				for (int j = i; j >= gap && abs(getCustomer(queue, j - gap, true)->energy) <= abs(getCustomer(queue, j, true)->energy); j -= gap)
				{
					if (abs(getCustomer(queue, j - gap, true)->energy) < abs(getCustomer(queue, j, true)->energy))
					{
						swapCustomer(getCustomer(queue, j, true), getCustomer(queue, j - gap, true));
						N++;
					}
					else
					{
						int pos1Origin = 0, pos2Origin = 0;

						customer *i = queueOrder;
						while (i->name != getCustomer(queue, j - gap, true)->name)
						{
							pos1Origin++;
							i = i->next;
						}

						i = queueOrder;
						while (i->name != getCustomer(queue, j, true)->name)
						{
							pos2Origin++;
							i = i->next;
						}

						if (pos1Origin > pos2Origin)
						{
							swapCustomer(getCustomer(queue, j, true), getCustomer(queue, j - gap, true));
							N++;
						}
					}
				}
			}
			gap /= 2;
		}

		cerr << N << endl;

		temp = queue;
		while (temp != nullptr)
		{
			cerr << temp->name << " " << temp->energy << endl;
			temp = temp->next;
		}

		cerr << "queueOrder \n";
		temp = queueOrder;
		while (temp != nullptr)
		{
			cerr << temp->name << " " << temp->energy << endl;
			temp = temp->next;
		}

		BLUE(N % MAXSIZE);

		cerr << "Table\n";
		temp = table;
		while (1)
		{
			cerr << temp->name << " " << temp->energy << endl;
			temp = temp->next;
			if (temp == table)
				break;
		}
		// print queue
		cerr << "Queue\n";
		temp = queue;
		while (temp != nullptr)
		{
			cerr << temp->name << " " << temp->energy << endl;
			temp = temp->next;
		}
		// print recent
		cerr << "Recent\n";
		temp = recent;
		while (temp != nullptr)
		{
			cerr << temp->name << " " << temp->energy << endl;
			temp = temp->next;
		}
		// print queueOrder
		cerr << "queueOrder \n";
		temp = queueOrder;
		while (temp != nullptr)
		{
			cerr << temp->name << " " << temp->energy << endl;
			temp = temp->next;
		}
	}

	void REVERSAL()
	{
		cerr << "REVERSAL check \n";
		if (table == nullptr || table->prev == nullptr)
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

		// free headPos and headNeg
		while (headPos != nullptr)
		{
			customer *temp = headPos;
			headPos = headPos->next;
			delete temp;
		}
		delete headPos;
		while (headNeg != nullptr)
		{
			customer *temp = headNeg;
			headNeg = headNeg->next;
			delete temp;
		}
		delete headNeg;

		temp = table;
		while (1)
		{
			cerr << temp->name << " " << temp->energy << endl;
			temp = temp->next;
			if (temp == table)
				break;
		}
	}

	void UNLIMITED_VOID()
	{
		cerr << "UNLIMITED VOID check \n";
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
				cout << temp->name << "-" << temp->energy << endl;
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
					cerr << tempSum << " " << sum << endl;
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

		cerr << sum << endl;

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
	}

	void DOMAIN_EXPANSION()
	{
		cerr << "DOMAIN EXPANSION check \n";
		if (table == nullptr || table->next == nullptr)
			return;
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
		if (queue != nullptr)
		{
			cerr << "queue removing \n";
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
			temp = queueOrder;
			while (temp->next != nullptr)
				temp = temp->next;
			// temp: tail of queue, most recent get in the restaurant
			// Start removing in queue
			while (temp != nullptr)
			{
				customer *tempTemp = temp;
				temp = temp->prev;
				if (ePos >= eNeg ? tempTemp->energy < 0 : tempTemp->energy > 0)
				{
					cout << tempTemp->name << "-" << tempTemp->energy << endl;
					string Name = tempTemp->name;
					remove(queueOrder, tempTemp);
					remove(queue, findCustomer(queue, Name), true);
				}
			}
		}

		cerr << "table removing \n";
		temp = recent;
		while (temp->next != nullptr)
			temp = temp->next;
		// temp : tail = most recent get in the table
		// Start removing in table and recent
		while (temp != nullptr)
		{
			if (ePos >= eNeg ? temp->energy < 0 : temp->energy > 0)
			{
				cout << temp->name << "-" << temp->energy << endl;
				remove(table, findCustomer(table, temp->name));
				customer *tempTemp = temp;
				temp = temp->prev;
				remove(recent, tempTemp);
			}
			else
				temp = temp->prev;
		}

		// Add customer from queue
		while (sizeTable < MAXSIZE && queue != nullptr)
		{
			RED(queue->name, queue->energy);
			remove(queueOrder, findCustomer(queueOrder, queue->name));
			remove(queue, nullptr, true);
		}

		cerr << "Table\n";
		temp = table;
		while (1)
		{
			cerr << temp->name << " " << temp->energy << endl;
			temp = temp->next;
			if (temp == table)
				break;
		}
		// print queue
		cerr << "Queue\n";
		temp = queue;
		while (temp != nullptr)
		{
			cerr << temp->name << " " << temp->energy << endl;
			temp = temp->next;
		}
		// print recent
		cerr << "Recent\n";
		temp = recent;
		while (temp != nullptr)
		{
			cerr << temp->name << " " << temp->energy << endl;
			temp = temp->next;
		}
		cerr << "queueOrder \n";
		temp = queueOrder;
		while (temp != nullptr)
		{
			cerr << temp->name << " " << temp->energy << endl;
			temp = temp->next;
		}
	}

	void LIGHT(int num)
	{
		cerr << "LIGHT check \n";
		if (num != 0)
		{
			if (table == nullptr)
				return;
			if (table->next == nullptr)
			{
				cout << table->name << "-" << table->energy << endl;
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
			while (1)
			{
				cout << temp->name << "-" << temp->energy << endl;
				temp = temp->prev;
				if (temp == table)
					break;
			}
		}
	}
	~imp_res()
	{
		for (int i = 0; i < sizeTable - 1; i++)
		{
			customer *temp = table;
			table = table->next;
			delete temp;
		}
		delete table;
		cerr << "deleted table \n";
		while (queue != nullptr)
		{
			customer *temp = queue;
			queue = queue->next;
			delete temp;
		}
		delete queue;
		cerr << "deleted queue \n";
		while (queueOrder != nullptr)
		{
			customer *temp = queueOrder;
			queueOrder = queueOrder->next;
			delete temp;
		}
		delete queueOrder;
		cerr << "deleted queueOrder \n";
		while (recent != nullptr)
		{
			customer *temp = recent;
			recent = recent->next;
			delete temp;
		}
		delete recent;
		cerr << "deleted recent \n";
	}
};