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

	//�������� �������� � �������� �������
	void Delete(int index) {
		//����� ������������ ����� � �������
		int desiredTableIndex = Search_entry_in_table(index);
		if (index > indexTable[desiredTableIndex].numberFirstElementInBlock) {
			Block* current = Search_block_and_index_in_table(index).first;
			int numberElement = Search_block_and_index_in_table(index).second;
			if (desiredTableIndex == 6) { //���� ��� ����� ������, �� �� ����� ����������
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
			if (desiredTableIndex == 0) { //���� ��� ������ ������, �� ��� ����� ����������
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

			if (desiredTableIndex == 0) { //���� ��� ������ ������, �� ��� ����� ����������
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
			else if (desiredTableIndex == 6) { //���� ��� ����� ������, �� �� ����� ����������
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

	//���������� �������� � �������� �������
	void Insert(Data& element, int index) {
		//����� ������������ ����� � �������
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
					if (current->pNext != nullptr) { //���� ��� �� �����
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
					else { //����� ��� �����
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
					if (current->pNext == nullptr) { //���� ����� ������
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

	//���������� �������� � ������ ������ (���� ���� ��������, �� ������ ��� ��������� ����� ����)
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
				//������� ������ �����
				Block* nextBlock = head->pNext;
				Block* newBlock = new Block;
				//����� ����� ����� ������ � ������� ������
				newBlock->pBack = head;
				head->pNext = newBlock;
				//����� ����� ������ � ��������� ������ (���� �� ����)
				if (nextBlock != nullptr) {
					newBlock->pNext = nextBlock;
					nextBlock->pBack = newBlock;
				}
				//������������ ��������� � ������� ������ �������� � ������
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

	//���������� �������� � ������ ������ (���� ��� ����� ���������, �� ������ ��� ��������� ����� ����)
	void InsertToHeadTwo(Data& element) {
		if (head == nullptr) { //���� ������ ����
			Block* newBlock = new Block;
			newBlock->data[0] = element;
			head = tail = newBlock;
			newBlock->fullness++;
			Size++;
		}
		else { //���� ������ ��� ����� �����
			if (head->fullness == 4) { //���� ������ ������ ��������� ������
				if (head->pNext == nullptr) { //���� ������ ������� ������ �� ������ �����
					//�������� ������ ����� � ��� ������������� � ������
					Block* twoBlock = new Block;
					twoBlock->pBack = head;
					head->pNext = twoBlock;
					tail = twoBlock;
					//������� ������ ��������
					tail->data[0] = head->data[3];
					tail->fullness++;
					for (int i = 3; i > 0; --i) {
						head->data[i] = head->data[i - 1];
					}
					head->data[0] = element;
					Size++;
				}
				else { //����� ������ ����� ��� ������� ��� �����
					Block* twoBlock = head->pNext;
					if (twoBlock->fullness == 4) { //���� ������ ���� ��������� �����
						Block* threeBlock = new Block;
						if (twoBlock->pNext == nullptr) { //���� ������ ����� ������ ��� �����
							//������������ ������ ����, ��� ����� ������
							twoBlock->pNext = threeBlock;
							threeBlock->pBack = twoBlock;
							tail = threeBlock;
							//���������� ��������
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
						else { //����� � ������ ���� ��� ������� ��� �����
							//������� ����� � ������ (����� �������)
							Block* fourBlock = twoBlock->pNext;
							threeBlock->pBack = twoBlock;
							threeBlock->pNext = fourBlock;
							fourBlock->pBack = threeBlock;
							twoBlock->pNext = threeBlock;
							//������� ��������
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
					else { //���� �� ������ ����� ���� �����
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
			else { //���� � ������ ������ ���� �����
				for (int i = head->fullness - 1; i >= 0; --i) {
					head->data[i + 1] = head->data[i];
				}
				head->data[0] = element;
				head->fullness++;
				Size++;
			}
		}
	}

	//���������� �������� � ����� ������
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

	//����� ����� �������
	void PrintList() {
		if (head == nullptr) {
			cout << endl << "������ ����!" << endl;
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

	//����� ��������� ������� ������
	void PrintIndexTable() {
		for (int i = 0; i < 7; ++i) {
			cout << setw(3) << indexTable[i].link << " | " << indexTable[i].numberBlock << " | " << indexTable[i].numberFirstElementInBlock << endl;
		}
	}

	//��������� �������� � �������� �������
	void SetElement(Data element, int index) {
		//����� ������������ ����� � ������� � ������ �� ������������ ����� � ������� � ���� �����
		pair<Block*, int> Block_and_index = Search_block_and_index_in_table(index);
		Block_and_index.first->data[Block_and_index.second] = element;
	}

	//������ ������ ������
	int GeSize() { return Size; }

	//������� ������
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

	//���������������� �����
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
		//����� ����������� ����� � ������
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
		if (fullnessIndexTable == 7) { // ���� ������� ������
			int k = 0;
			//������ �� ������� (������� ������ ��������� � �����)
			//��������� ������ ����, �� �������� ����� ����������/�������� �������
			while (indexElement >= indexTable[k].numberFirstElementInBlock && k != 6) {
				if (indexElement == indexTable[k].numberFirstElementInBlock) {
					break;
				}
				k++;
			}
			if (indexElement > indexTable[k].numberFirstElementInBlock) {
				if (operation == '+') {
					if (indexTable[6].link != tail) { //���� ��������� ����� ������ (+����)
						indexTable[6].link = tail;
						indexTable[6].numberBlock++;
						indexTable[6].numberFirstElementInBlock += 4;
					}
					else { //����� ����� ������ �� ���������
						for (int i = k + 1; i < 7; ++i) {
							indexTable[i].numberFirstElementInBlock++;
							if (changeBlock == '+') { indexTable[i].numberBlock++; }
						}
					}
				}
				else {
					//� ������ �������� ����� ������ �� ����� ���������. ����� ��������� ����� ����� ������� ��������� ������� �� ������ ������
					//� ����� ��� �������, ����� ������� �������, ������� ����� ����� ������� ������� �������� ����� � �������, �.�. 
					// indexElement <= indexTable[k].numberFirstElementInBlock
					for (int i = k + 1; i < 7; ++i) {
						indexTable[i].numberFirstElementInBlock++;
						if (changeBlock == '-') { indexTable[i].numberBlock--; }
					}
				}
			}
			else if (indexElement <= indexTable[k].numberFirstElementInBlock) {
				if (operation == '+') {
					if (indexTable[6].link != tail) { //���� ��������� ����� ������
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
					//��������: �������� ���� �� ������� ���� ������ � ������� ��� ���
					//r - ������ �� ����������� ���� � �������
					bool flag = false;
					int r;
					for (r = 0; r < 7; ++r) {
						if (indexTable[r].link == deleteBlock) {
							flag = true;
							break;
						}
					}
					//����� ����� ��������� ����� ������
					if (indexTable[6].link != tail) { //���� ��������� ����� ������ (-����)
						if (indexTable[5].link != tail) { //���� 5-�� ������ (�� �������) � ������� �� �������� ����� ����� (�.�. ���� ����� �� ��������� ������ �� ���� �����)
							indexTable[6].link = tail;
							indexTable[6].numberBlock--;
							indexTable[6].numberFirstElementInBlock -= tail->fullness;
						}
						else { //����� ����� ���������� �����, ������� ������� 5 ������� ������ �����
							if (indexTable[4].link == indexTable[5].link->pBack) {
								cout << "������!";
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
						if ((r != 0) && flag == true) { //���� �������� ����, �� ������� ���� ������ � ��������� �������
							if (deleteBlock->pBack == indexTable[r - 1].link) {
								if (deleteBlock->pBack->pBack == indexTable[r - 2].link || deleteBlock->pBack == head) {
									cout << "������!";
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
						if (indexTable[0].link != head) { //���� ���������� ������ ������
							indexTable[0].link = head;
							indexTable[0].numberBlock = 0;
							indexTable[0].numberFirstElementInBlock = 0;
						}
					}
				}
				if (deleteBlock != nullptr) { delete deleteBlock; }
			}
		}
		else { //����� ������� ��������. ������� ��� ������ ��������� ������, ���������� ������ �� ����� ������
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
	cout << endl << "�������� �������:" << endl;
	cout << "[1] - �������� ����� ������ � ������ ������" << endl;
	cout << "[2] - �������� ����� ������ � ����� ������" << endl;
	cout << "[3] - �������� ����� ������ � �������� �������" << endl;
	cout << "[4] - ������� ������ � �������� �������" << endl;
	cout << "[5] - �������� ������ � �������� �������" << endl;
	cout << "[6] - �������� ���� ������" << endl;
	cout << "[7] - ����� � ������" << endl;
	cout << "[8] - ����� ����� ������" << endl;
	cout << "[9] - ����� ��������� ������� ������" << endl;
	cout << "[10] - ������ ������" << endl;
	cout << "[11] - ��������� ������" << endl;
	cout << "���� �������: ";
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
			cout << "������� ������:" << endl;
			cin >> data;
			lst.InsertToHead
			(data);
		}
		else if (command == 2) {
			Data data;
			cout << "������� ������:" << endl;
			cin >> data;
			cout << endl;
			lst.InsertToTail(data);
		}
		else if (command == 3) {
			Data data;
			cout << "������� ������:" << endl;
			cin >> data;
			int id = 0;
			cout << "������� �������:" << endl;
			cin >> id;
			lst.Insert(data, id);
		}
		else if (command == 4) {
			int id = 0;
			cout << "������� �������:" << endl;
			cin >> id;
			lst.Delete(id);
		}
		else if (command == 5) {
			Data data;
			cout << "������� ������:" << endl;
			cin >> data;
			int id = 0;
			cout << "������� �������:" << endl;
			cin >> id;
			lst.SetElement(data, id);
		}
		else if (command == 6) {
			lst.Clear();
		}
		else if (command == 7) {
			float arg[3];
			cout << "������� �������� ������:" << endl;
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
		else { cout << endl << "����������� �������!" << endl; }
		PrintMenu();
	}
	return 0;
}
