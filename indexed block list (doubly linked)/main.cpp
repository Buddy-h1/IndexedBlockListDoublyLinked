#include<iostream>
#include<iomanip>
#include<utility>
#include<vector>

using namespace std;

class Data {
public:
	Data() {
		for (int i = 0; i < 3; ++i) {
			a[i] = 0;
			b[i] = 0;
			c[i] = 'E';
		}
		d = 0;
	}
	Data(float a[3], int b[3], char c[3], float d) {
		for (int i = 0; i < 3; ++i) {
			this->a[i] = a[i];
			this->b[i] = b[i];
			this->c[i] = c[i];
		}
		this->d = d;
	}
	float a[3];
	int b[3];
	char c[3];
	float d;
};

istream& operator>>(istream& stream, Data& data) {
	for (int i = 0; i < 3; ++i) {
		stream >> data.a[i];
	}
	for (int i = 0; i < 3; ++i) {
		stream >> data.b[i];
	}
	for (int i = 0; i < 3; ++i) {
		stream >> data.c[i];
	}
	stream >> data.d;
	return stream;
}

ostream& operator<<(ostream& stream, Data& data) {
	for (int i = 0; i < 3; ++i) {
		cout << data.a[i] << " ";
	}
	cout << "| ";
	for (int i = 0; i < 3; ++i) {
		cout << data.b[i] << " ";
	}
	cout << "| ";
	for (int i = 0; i < 3; ++i) {
		cout << data.c[i] << " ";
	}
	cout << "| ";
	cout << data.d;
	return stream;
}

bool operator==(Data lhs, Data rhs) {
	bool flag = false;
	for (int i = 0; i < 3; ++i) {
		if ((lhs.a[i] == rhs.a[i]) && (lhs.b[i] == rhs.b[i]) && (lhs.c[i] == rhs.c[i]) && (lhs.d == rhs.d)) {
			flag = true;
		}
	}
	return flag;
}

bool operator!=(Data lhs, Data rhs) {
	if (lhs == rhs) { return false; }
	else { return true; }
}

class List {
public:
	List() {
		head = nullptr;
		tail = nullptr;
		for (auto& i : indexTable) {
			i.link = nullptr;
			i.numberBlock = 0;
			i.numberFirstElementInBlock = 0;
		}
		Size = 0;
		fullnessIndexTable = 0;
	}

	//Удаление элемента в заданной позиции
	void Delete(int index) {
		//Поиск оптимального входа в таблицу
		int desiredTableIndex = Search_entry_in_table(index);
		if (index > indexTable[desiredTableIndex].numberFirstElementInBlock) {
			Block* current = Search_block_and_index_in_table(index).first;
			int numberElement = Search_block_and_index_in_table(index).second;
			if (desiredTableIndex == 6) { //Если это хвост списка, то он может измениться
				Block* current = tail;
				int numberElement = tail->fullness - 1;
				for (int i = numberElement; i < 3; ++i) {
					current->data[i] = current->data[i + 1];
				}
				current->data[current->fullness - 1] = Data();
				current->fullness--;
				Size--;
				if (tail->fullness == 0) {
					Block* deleteBlock = tail;
					if (tail->pBack != nullptr) {
						tail = tail->pBack;
						tail->pNext = nullptr;
					}
					delete deleteBlock;
				}
				UpdatingTable(index, '-', '-');
			}
			else {
				for (int i = numberElement; i < 3; ++i) {
					current->data[i] = current->data[i + 1];
				}
				current->data[current->fullness - 1] = Data();
				current->fullness--;
				Size--;
				if (current->fullness == 0) {
					Block* deleteBlock = current;
					Block* backBlock = current->pBack;
					Block* nextBlock = current->pNext;
					backBlock->pNext = nextBlock;
					nextBlock->pBack = backBlock;
					UpdatingTable(index, '-', '-', current);
				}
				else { UpdatingTable(index, '-', '+'); }
			}
		}
		else if (index < indexTable[desiredTableIndex].numberFirstElementInBlock) {
			Block* current = Search_block_and_index_in_table(index).first;
			int numberElement = Search_block_and_index_in_table(index).second;
			if (desiredTableIndex == 0) { //Если это голова списка, то она может измениться
				Block* current = head;
				int numberElement = index;
				for (int i = numberElement; i < 3; ++i) {
					current->data[i] = current->data[i + 1];
				}
				current->data[current->fullness - 1] = Data();
				current->fullness--;
				Size--;
				if (head->fullness == 0) {
					Block* deleteBlock = head;
					if (head->pNext != nullptr) {
						head = head->pNext;
					}
					delete deleteBlock;
					UpdatingTable(index, '-', '-', deleteBlock);
				}
				else { UpdatingTable(index, '-', '+'); }
			}
			else {
				for (int i = numberElement; i < 3; ++i) {
					current->data[i] = current->data[i + 1];
				}
				current->data[current->fullness - 1] = Data();
				current->fullness--;
				Size--;
				if (current->fullness == 0) {
					Block* deleteBlock = current;
					Block* backBlock = current->pBack;
					Block* nextBlock = current->pNext;
					backBlock->pNext = nextBlock;
					nextBlock->pBack = backBlock;
					UpdatingTable(index, '-', '-', current);
				}
				else { UpdatingTable(index, '-', '+'); }
			}
		}
		else {
			Block* current = indexTable[desiredTableIndex].link;
			int numberElement = 0;

			if (desiredTableIndex == 0) { //Если это голова списка, то она может измениться
				Block* current = head;
				int numberElement = index;
				for (int i = numberElement; i < 3; ++i) {
					current->data[i] = current->data[i + 1];
				}
				current->data[current->fullness - 1] = Data();
				current->fullness--;
				Size--;
				Block* deleteBlock = nullptr;
				bool flag = false;
				if (head->fullness == 0) {
					Block* deleteBlock = head;
					flag = true;
					if (head->pNext != nullptr) {
						head = head->pNext;
					}
				}
				if (flag == false) {
					UpdatingTable(index, '-', '+');
				}
				else {
					UpdatingTable(index, '-', '-', deleteBlock);
				}
			}
			else if (desiredTableIndex == 6) { //Если это хвост списка, то он может измениться
				Block* current = tail;
				int numberElement = tail->fullness - 1;
				for (int i = numberElement; i < 3; ++i) {
					current->data[i] = current->data[i + 1];
				}
				current->data[current->fullness - 1] = Data();
				current->fullness--;
				Size--;
				if (tail->fullness == 0) {
					Block* deleteBlock = tail;
					if (tail->pBack != nullptr) {
						tail = tail->pBack;
						tail->pNext = nullptr;
					}
					delete deleteBlock;
				}
				UpdatingTable(index, '-', '-');
			}
			else {
				for (int i = numberElement; i < 3; ++i) {
					current->data[i] = current->data[i + 1];
				}
				current->data[current->fullness - 1] = Data();
				current->fullness--;
				Size--;
				if (current->fullness == 0) {
					Block* deleteBlock = current;
					Block* backBlock = current->pBack;
					Block* nextBlock = current->pNext;
					backBlock->pNext = nextBlock;
					nextBlock->pBack = backBlock;
					UpdatingTable(index, '-', '-', current);
				}
				else { UpdatingTable(index, '-', '+'); }
			}
		}
	}

	//Добавление элемента в заданную позицию
	void Insert(Data& element, int index) {
		//Поиск оптимального входа в таблицу
		int desiredTableIndex = Search_entry_in_table(index);
		if (index == 0) {
			InsertToHead(element);
		}
		else if (index >= Size) {
			int difference = index - Size;
			Data maxElement = { new float[3]{2000000000.0, 2000000000.0, 2000000000.0}, new int[3]{2000000000,2000000000,2000000000}, new char[3]{'a', 'a', 'a'}, 2000000000.0 };
			for (int i = 0; i < difference; ++i) {
				InsertToTail(maxElement);
			}
			InsertToTail(element);
		}
		else {
			if (index > indexTable[desiredTableIndex].numberFirstElementInBlock) {
				Block* current = Search_block_and_index_in_table(index).first;
				int numberElement = Search_block_and_index_in_table(index).second;
				if (current->fullness == 4) {
					Block* new_block = new Block;
					if (current->pNext != nullptr) { //Если это не хвост
						Block* nextBlock = current->pNext;
						new_block->pBack = current;
						new_block->pNext = nextBlock;
						current->pNext = new_block;
						nextBlock->pBack = new_block;
						new_block->data[0] = current->data[3];
						new_block->fullness++;
						for (int i = 3; i > numberElement; --i) {
							current->data[i] = current->data[i - 1];
						}
						current->data[numberElement] = element;
						Size++;
						UpdatingTable(index, '+', '+');
					}
					else { //Иначе это хвост
						Block* new_block = new Block;
						new_block->pBack = current;
						current->pNext = new_block;
						tail = new_block;
						new_block->data[0] = current->data[3];
						new_block->fullness++;
						for (int i = 3; i > numberElement; --i) {
							current->data[i] = current->data[i - 1];
						}
						current->data[numberElement] = element;
						Size++;
						UpdatingTable(index, '+', '+');
					}
				}
				else {
					for (int i = 3; i > numberElement; --i) {
						current->data[i] = current->data[i - 1];
					}
					current->data[numberElement] = element;
					current->fullness++;
					Size++;
					UpdatingTable(index, '+', '-');
				}
			}
			else if (index < indexTable[desiredTableIndex].numberFirstElementInBlock) {
				Block* current = Search_block_and_index_in_table(index).first;
				int numberElement = Search_block_and_index_in_table(index).second;
				if (current->fullness == 4) {
					Block* new_block = new Block;
					Block* three_block = current->pNext;
					new_block->pBack = current;
					new_block->pNext = three_block;
					three_block->pBack = new_block;
					current->pNext = new_block;
					new_block->data[0] = current->data[3];
					new_block->fullness++;
					for (int i = 3; i > numberElement; --i) {
						current->data[i] = current->data[i - 1];
					}
					current->data[numberElement] = element;
					Size++;
					UpdatingTable(index, '+', '+');
				}
				else {
					for (int i = 3; i > numberElement; --i) {
						current->data[i] = current->data[i - 1];
					}
					current->data[numberElement] = element;
					current->fullness++;
					Size++;
					UpdatingTable(index, '+', '-');
				}
			}
			else {
				Block* current = indexTable[desiredTableIndex].link;
				if (current->fullness == 4) {
					if (current->pNext == nullptr) { //Если хвост списка
						Block* new_block = new Block;
						tail = new_block;
						tail->pBack = current;
						current->pNext = tail;
						tail->data[0] = current->data[3];
						tail->fullness++;
						for (int i = 3; i > 0; --i) {
							current->data[i] = current->data[i - 1];
						}
						current->data[0] = element;
						Size++;
						UpdatingTable(index, '+', '+');
					}
					else {
						Block* new_block = new Block;
						Block* three_block = current->pNext;
						new_block->pBack = current;
						new_block->pNext = three_block;
						three_block->pBack = new_block;
						current->pNext = new_block;
						new_block->data[0] = current->data[3];
						new_block->fullness++;
						for (int i = 3; i > 0; --i) {
							current->data[i] = current->data[i - 1];
						}
						current->data[0] = element;
						Size++;
						UpdatingTable(index, '+', '+');
					}
				}
				else {
					for (int i = 3; i > 0; --i) {
						current->data[i] = current->data[i - 1];
					}
					current->data[0] = element;
					current->fullness++;
					Size++;
					UpdatingTable(index, '+', '-');
				}
			}
		}
	}

	//Добавление элемента в начало списка (если блок заполнен, то каждый раз создается новый блок)
	void InsertToHead(Data& element) {
		if (head == nullptr) {
			Block* newBlock = new Block;
			newBlock->data[0] = element;
			head = tail = newBlock;
			newBlock->fullness++;
			Size++;
			UpdatingTable(0, '+', '+');
		}
		else {
			if (head->fullness != 4) {
				for (int i = 3; i > 0; --i) { head->data[i] = head->data[i - 1]; }
				head->data[0] = element;
				head->fullness++;
				Size++;
				UpdatingTable(0, '+', '-');
			}
			else {
				//Вставка нового блока
				Block* nextBlock = head->pNext;
				Block* newBlock = new Block;
				//Связь между новым блоком и головой списка
				newBlock->pBack = head;
				head->pNext = newBlock;
				//Связь между блоком и следующим блоком (если он есть)
				if (nextBlock != nullptr) {
					newBlock->pNext = nextBlock;
					nextBlock->pBack = newBlock;
				}
				//Передвижение элементов и вставка нового элемента в начало
				newBlock->data[0] = head->data[3];
				newBlock->fullness++;
				for (int i = 3; i > 0; --i) { head->data[i] = head->data[i - 1]; }
				head->data[0] = element;
				Block* current = head;
				while (current->pNext != nullptr) { current = current->pNext; }
				tail = current;
				Size++;
				UpdatingTable(0, '+', '+');
			}
		}
	}

	//Добавление элемента в начало списка (если два блока заполнено, то каждый раз создается новый блок)
	void InsertToHeadTwo(Data& element) {
		if (head == nullptr) { //Если список пуст
			Block* newBlock = new Block;
			newBlock->data[0] = element;
			head = tail = newBlock;
			newBlock->fullness++;
			Size++;
		}
		else { //Если список уже имеет блоки
			if (head->fullness == 4) { //Если голова списка полностью забита
				if (head->pNext == nullptr) { //Если список состоит только из одного блока
					//Создание нового блока и его присоединение к списку
					Block* twoBlock = new Block;
					twoBlock->pBack = head;
					head->pNext = twoBlock;
					tail = twoBlock;
					//Вставка нового элемента
					tail->data[0] = head->data[3];
					tail->fullness++;
					for (int i = 3; i > 0; --i) {
						head->data[i] = head->data[i - 1];
					}
					head->data[0] = element;
					Size++;
				}
				else { //Иначе список имеет как минимум два блока
					Block* twoBlock = head->pNext;
					if (twoBlock->fullness == 4) { //Если второй блок полностью забит
						Block* threeBlock = new Block;
						if (twoBlock->pNext == nullptr) { //Если список имеет только два блока
							//Присоединить третий блок, как хвост списка
							twoBlock->pNext = threeBlock;
							threeBlock->pBack = twoBlock;
							tail = threeBlock;
							//Добавление элемента
							tail->data[0] = twoBlock->data[3];
							tail->fullness++;
							for (int i = 3; i > 0; --i) {
								twoBlock->data[i] = twoBlock->data[i - 1];
							}
							twoBlock->data[0] = head->data[3];
							for (int i = 3; i > 0; --i) {
								head->data[i] = head->data[i - 1];
							}
							head->data[0] = element;
							Size++;
						}
						else { //Иначе в списке есть как минимум три блока
							//Вставка блока в список (между блоками)
							Block* fourBlock = twoBlock->pNext;
							threeBlock->pBack = twoBlock;
							threeBlock->pNext = fourBlock;
							fourBlock->pBack = threeBlock;
							twoBlock->pNext = threeBlock;
							//Вставка элемента
							threeBlock->data[0] = twoBlock->data[3];
							threeBlock->fullness++;
							for (int i = 3; i > 0; --i) {
								twoBlock->data[i] = twoBlock->data[i - 1];
							}
							twoBlock->data[0] = head->data[3];
							for (int i = 3; i > 0; --i) {
								head->data[i] = head->data[i - 1];
							}
							head->data[0] = element;
							Size++;
						}
					}
					else { //Если во втором блоке есть место
						for (int i = twoBlock->fullness - 1; i >= 0; --i) {
							twoBlock->data[i + 1] = twoBlock->data[i];
						}
						twoBlock->data[0] = head->data[3];
						twoBlock->fullness++;
						for (int i = 3; i > 0; --i) {
							head->data[i] = head->data[i - 1];
						}
						head->data[0] = element;
						Size++;
					}
				}
			}
			else { //Если в голове списка есть место
				for (int i = head->fullness - 1; i >= 0; --i) {
					head->data[i + 1] = head->data[i];
				}
				head->data[0] = element;
				head->fullness++;
				Size++;
			}
		}
	}

	//Добавление элемента в конец списка
	void InsertToTail(Data& element) {
		if (head == nullptr) {
			Block* newBlock = new Block;
			newBlock->data[0] = element;
			head = tail = newBlock;
			newBlock->fullness++;
			Size++;
			UpdatingTable(0, '+', '+');
		}
		else {
			Block* current = tail;
			if (current->fullness == 4) {
				Block* newBlock = new Block;
				newBlock->data[0] = element;
				newBlock->pBack = tail;
				tail->pNext = newBlock;
				tail = newBlock;
				newBlock->fullness++;
				Size++;
				UpdatingTable(1000000, '+', '+');
			}
			else {
				Data defaultData = Data();
				for (int i = 0; i < 4; ++i) {
					if (current->data[i] == defaultData) {
						current->data[i] = element;
						current->fullness++;
						break;
					}
				}
				Size++;
				UpdatingTable(1000000, '+', '-');
			}
		}
	}

	//Вывод всего массива
	void PrintList() {
		if (head == nullptr) {
			cout << endl << "Список пуст!" << endl;
		}
		else {
			cout << endl;
			Block* current = head;
			while (current != nullptr) {
				for (auto& i : current->data) {
					cout << i;
					cout << endl;
				}
				cout << endl;
				current = current->pNext;
			}
		}
		/*else {
			Block* current = head;
			while (current != nullptr) {
				for (int i = 0; i < current->fullness; ++i) {
					cout << current->data[i] << endl;
				}
				current = current->pNext;
			}
			cout << endl;
		}*/
	}

	//Вывод индексной таблицы списка
	void PrintIndexTable() {
		for (int i = 0; i < 7; ++i) {
			cout << setw(3) << indexTable[i].link << " | " << indexTable[i].numberBlock << " | " << indexTable[i].numberFirstElementInBlock << endl;
		}
	}

	//Изменение элемента в заданной позиции
	void SetElement(Data element, int index) {
		//Поиск оптимального входа в таблицу и проход до необходимого блока и индекса в этом блоке
		pair<Block*, int> Block_and_index = Search_block_and_index_in_table(index);
		Block_and_index.first->data[Block_and_index.second] = element;
	}

	//Узнать размер списка
	int GeSize() { return Size; }

	//Очистка списка
	void Clear() {
		Block* temp = head;
		while (temp != tail) {
			Block* deleteBlock = temp;
			temp = temp->pNext;
			delete deleteBlock;
		}
		head = nullptr;
		fullnessIndexTable = 0;
		for (int i = 0; i < 7; ++i) {
			indexTable[i] = Table();
		}
		Size = 0;
	}

	//Последовательный поиск
	vector<Data> Search(float arg1, float arg2, float arg3) {
		Block* current = head;
		vector<Data> result;
		while (current != tail) {
			for (int i = 0; i < current->fullness; ++i) {
				if (current->data[i].a[0] == arg1 &&
					current->data[i].a[1] == arg2 &&
					current->data[i].a[2] == arg3) {
					result.push_back(current->data[i]);
				}
			}
			current = current->pNext;
		}
		return result;
	}

private:

	int Search_entry_in_table(int index) {
		//Поиск подходящего входа в список
		Table defaultTable = Table();
		int desiredTableIndex = 0;
		int arrayDifference[7];
		for (int i = 0; i < 7; ++i) {
			arrayDifference[i] = abs(index - indexTable[i].numberFirstElementInBlock);
		}
		int min = 1000;
		for (int i = 0; i < 7; ++i) {
			if (arrayDifference[i] < min && arrayDifference[i] >= 0) {
				desiredTableIndex = i;
				min = arrayDifference[i];
			}
		}
		return desiredTableIndex;
	}

	class Block {
	public:
		Block() {
			this->pBack = nullptr;
			this->pNext = nullptr;
			this->fullness = 0;
			for (auto i : this->data) {
				i = Data();
			}
		}
		Data data[4];
		Block* pBack;
		Block* pNext;
		int fullness;
		~Block() {
			pBack = nullptr;
			pNext = nullptr;
		}
	};

	pair<Block*, int> Search_block_and_index_in_table(int index) {
		int desiredTableIndex = Search_entry_in_table(index);
		if (index > indexTable[desiredTableIndex].numberFirstElementInBlock) {
			int numberElement = indexTable[desiredTableIndex].numberFirstElementInBlock;
			Block* current = indexTable[desiredTableIndex].link;
			Data defaultData = Data();
			while (numberElement != index) {
				bool flag = false;
				for (int i = 0; i < current->fullness; ++i) {
					if (current->data[i] != defaultData) {
						if (numberElement == index) {
							numberElement = i;
							flag = true;
							break;
						}
						numberElement++;
						if (i == 3) {
							current = current->pNext;
							if (numberElement == index) {
								numberElement = 0;
								flag = true;
								break;
							}
						}
					}
				}
				if (flag == true) { break; }
			}
			return { current, numberElement };
		}
		else if (index < indexTable[desiredTableIndex].numberFirstElementInBlock) {
			int numberElement = indexTable[desiredTableIndex].numberFirstElementInBlock;
			Block* current = indexTable[desiredTableIndex].link;
			Data defaultData = Data();
			while (numberElement != index) {
				bool flag = false;
				for (int i = 3; i >= 0; --i) {
					if (current->data[i] != defaultData) {
						numberElement--;
						if (i == 0) {
							current = current->pBack;
							if (numberElement == index) {
								for (int k = 3; k >= 0; --k) {
									if (current->data[k] != defaultData) {
										numberElement = k;
										flag = true;
										break;
									}
								}
								if (flag == true) { break; }
							}
						}
						if (numberElement == index) {
							numberElement = i;
							flag = true;
							break;
						}
					}
				}
				if (flag == true) { break; }
			}
			return { current, numberElement };
		}
		else {
			int numberElement = 0;
			Block* current = indexTable[desiredTableIndex].link;
			return { current, numberElement };
		}
	}

	class Table {
	public:
		Table() {
			numberBlock = 0;
			numberFirstElementInBlock = 0;
			link = nullptr;
		}
		int numberBlock;
		int numberFirstElementInBlock;
		Block* link;
	};

	void UpdatingTable(int indexElement, char operation, char changeBlock, Block* deleteBlock = nullptr) {
		if (fullnessIndexTable == 7) { // Если таблица полная
			int k = 0;
			//Проход по таблице (индексы первых элементов в блоке)
			//Находится нужный блок, от которого нужно прибавлять/убавлять индексы
			while (indexElement >= indexTable[k].numberFirstElementInBlock && k != 6) {
				if (indexElement == indexTable[k].numberFirstElementInBlock) {
					break;
				}
				k++;
			}
			if (indexElement > indexTable[k].numberFirstElementInBlock) {
				if (operation == '+') {
					if (indexTable[6].link != tail) { //Если изменился хвост списка (+блок)
						indexTable[6].link = tail;
						indexTable[6].numberBlock++;
						indexTable[6].numberFirstElementInBlock += 4;
					}
					else { //Иначе хвост списка не изменился
						for (int i = k + 1; i < 7; ++i) {
							indexTable[i].numberFirstElementInBlock++;
							if (changeBlock == '+') { indexTable[i].numberBlock++; }
						}
					}
				}
				else {
					//В данной ситуации хвост списка не может изменится. Чтобы изменился хвост нужно удалить последний элемент из хвоста списка
					//А чтобы это сделать, нужно удалить элемент, который будет равен индексу первого элемента блока в таблице, т.е. 
					// indexElement <= indexTable[k].numberFirstElementInBlock
					for (int i = k + 1; i < 7; ++i) {
						indexTable[i].numberFirstElementInBlock++;
						if (changeBlock == '-') { indexTable[i].numberBlock--; }
					}
				}
			}
			else if (indexElement <= indexTable[k].numberFirstElementInBlock) {
				if (operation == '+') {
					if (indexTable[6].link != tail) { //Если изменился хвост списка
						indexTable[6].link = tail;
						indexTable[6].numberBlock++;
						indexTable[6].numberFirstElementInBlock += 4;
					}
					else {
						if (indexElement == indexTable[k].numberFirstElementInBlock) {
							for (int i = k + 1; i < 7; ++i) {
								indexTable[i].numberFirstElementInBlock++;
								if (changeBlock == '+') { indexTable[i].numberBlock++; }
							}
						}
						else {
							for (int i = k; i < 7; ++i) {
								indexTable[i].numberFirstElementInBlock++;
								if (changeBlock == '+') { indexTable[i].numberBlock++; }
							}
						}
					}
				}
				else {
					//Проверка: удалился блок на который была сслыка в таблице или нет
					//r - индекс на удалившийся блок в таблице
					bool flag = false;
					int r;
					for (r = 0; r < 7; ++r) {
						if (indexTable[r].link == deleteBlock) {
							flag = true;
							break;
						}
					}
					//Здесь может изменится хвост списка
					if (indexTable[6].link != tail) { //Если изменился хвост списка (-блок)
						if (indexTable[5].link != tail) { //Если 5-ой строке (по индексу) в таблице НЕ присвоен новый хвост (т.е. если блоки из индексной аблицы не были рядом)
							indexTable[6].link = tail;
							indexTable[6].numberBlock--;
							indexTable[6].numberFirstElementInBlock -= tail->fullness;
						}
						else { //Иначе блоки находились рядом, поэтому сдвинем 5 элемент списка влево
							if (indexTable[4].link == indexTable[5].link->pBack) {
								cout << "Ошибка!";
								exit(0);
							}
							else {
								indexTable[5].link = indexTable[5].link->pBack;
								indexTable[5].numberBlock--;
								indexTable[5].numberFirstElementInBlock -= indexTable[5].link->pBack->fullness;
								indexTable[6].link = tail;
								indexTable[6].numberBlock--;
								indexTable[6].numberFirstElementInBlock -= tail->fullness;
							}
						}
					}
					else {
						if ((r != 0) && flag == true) { //Если удалился блок, на который была ссылка в индексной таблице
							if (deleteBlock->pBack == indexTable[r - 1].link) {
								if (deleteBlock->pBack->pBack == indexTable[r - 2].link || deleteBlock->pBack == head) {
									cout << "Ошибка!";
									exit(0);
								}
								else {
									indexTable[r - 1].link = indexTable[r - 1].link->pBack;
									indexTable[r - 1].numberBlock--;
									indexTable[r - 1].numberFirstElementInBlock -= indexTable[r - 1].link->pBack->fullness;
									indexTable[r].link = indexTable[r].link->pBack;
									indexTable[r].numberBlock--;
									indexTable[r].numberFirstElementInBlock -= indexTable[r].link->pBack->fullness;
								}
							}
							else {
								indexTable[r].link = indexTable[r].link->pBack;
								indexTable[r].numberBlock--;
								indexTable[r].numberFirstElementInBlock -= indexTable[r].link->pBack->fullness;
							}
						}
						if (indexElement == indexTable[k].numberFirstElementInBlock || flag == true) {
							for (int i = k + 1; i < 7; ++i) {
								indexTable[i].numberFirstElementInBlock--;
								if (changeBlock == '-') { indexTable[i].numberBlock--; }
							}
						}
						else {
							for (int i = k; i < 7; ++i) {
								indexTable[i].numberFirstElementInBlock--;
								if (changeBlock == '+') { indexTable[i].numberBlock--; }
							}
						}
						if (indexTable[0].link != head) { //Если изменилась голова списка
							indexTable[0].link = head;
							indexTable[0].numberBlock = 0;
							indexTable[0].numberFirstElementInBlock = 0;
						}
					}
				}
				if (deleteBlock != nullptr) { delete deleteBlock; }
			}
		}
		else { //Иначе таблица неполная. Поэтому при каждом изменении списка, происходит проход по всему списку
			Block* current = head;
			for (int i = 0; i < 7; ++i) {
				indexTable[i] = Table();
			}
			int countBlock = -1;
			int countElement = 0;
			int i = 0;
			while (current != nullptr) {
				indexTable[i].link = current;
				indexTable[i].numberFirstElementInBlock = countElement;
				countElement += current->fullness;
				countBlock++;
				indexTable[i].numberBlock = countBlock;
				i++;
				current = current->pNext;
			}
			fullnessIndexTable = i;
		}
	}

	Block* head;
	Block* tail;
	Table indexTable[7];
	int fullnessIndexTable;
	int Size;
};

void PrintMenu() {
	cout << endl << "Выберите команду:" << endl;
	cout << "[1] - Добавить новую запись в начало списка" << endl;
	cout << "[2] - Добавить новую запись в конец списка" << endl;
	cout << "[3] - Добавить новую запись в заданную позицию" << endl;
	cout << "[4] - Удалить запись в заданной позиции" << endl;
	cout << "[5] - Изменить запись в заданной позиции" << endl;
	cout << "[6] - Очистить весь список" << endl;
	cout << "[7] - Поиск в списке" << endl;
	cout << "[8] - Вывод всего списка" << endl;
	cout << "[9] - Вывод индексной таблицы списка" << endl;
	cout << "[10] - Размер списка" << endl;
	cout << "[11] - Завершить работу" << endl;
	cout << "Ваша команда: ";
}

int main() {
	setlocale(LC_ALL, "Russian");

	List lst;
	Data temp;

	int command = 0;
	PrintMenu();
	while (cin >> command) {
		if (command == 1) {
			Data data;
			cout << "Введите данные:" << endl;
			cin >> data;
			lst.InsertToHead
			(data);
		}
		else if (command == 2) {
			Data data;
			cout << "Введите данные:" << endl;
			cin >> data;
			cout << endl;
			lst.InsertToTail(data);
		}
		else if (command == 3) {
			Data data;
			cout << "Введите данные:" << endl;
			cin >> data;
			int id = 0;
			cout << "Введите позицию:" << endl;
			cin >> id;
			lst.Insert(data, id);
		}
		else if (command == 4) {
			int id = 0;
			cout << "Введите позицию:" << endl;
			cin >> id;
			lst.Delete(id);
		}
		else if (command == 5) {
			Data data;
			cout << "Введите данные:" << endl;
			cin >> data;
			int id = 0;
			cout << "Введите позицию:" << endl;
			cin >> id;
			lst.SetElement(data, id);
		}
		else if (command == 6) {
			lst.Clear();
		}
		else if (command == 7) {
			float arg[3];
			cout << "Введите аргумент поиска:" << endl;
			float arg1, arg2, arg3;
			cin >> arg1 >> arg2 >> arg3;
			vector<Data> result = lst.Search(arg1, arg2, arg3);
			for (auto& i : result) {
				cout << i << endl;
			}
			cout << endl;
		}
		else if (command == 8) {
			lst.PrintList();
		}
		else if (command == 9) {
			lst.PrintIndexTable();
		}
		else if (command == 10) {
			cout << lst.GeSize() << endl;
		}
		else if (command == 11) {
			return 0;
		}
		else { cout << endl << "Неизвестная команда!" << endl; }
		PrintMenu();
	}
	return 0;
}
