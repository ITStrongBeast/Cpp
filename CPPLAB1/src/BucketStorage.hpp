#pragma once

#include <cstdint>
#include <algorithm>

#include "Block.hpp"
#include "Node.hpp"

template<typename T>
class BucketStorage
{
public:
	// конструкторы контейнера
	BucketStorage() = default;

	BucketStorage(T block_capaсity)
	{
		this->block_capaсity = block_capaсity;
	}

	~BucketStorage() = default;

	// итератор контейнера
	class const_iterator
	{
		friend class BucketStorage;
		Node< T > *node = nullptr;

	public:
		const_iterator(Node< T > *node)
		{
			this->node = node;
		}

		const_iterator &operator=(const const_iterator &other)
		{
			this->node = other.node;
			return *this;
		}

		const_iterator &operator++()
		{
			if (node->next == node->block->capaсity)
			{
				if (node->block->next == nullptr)
				{
					// ошибочка
				}
				Block< T > *newBlock = node->block->next; // переход к следующему блоку
				node = newBlock->data[newBlock->first];   // переход к первому элементу в новом блоке
			}
			else
			{
				node = node->block->data[node->next];
			}
			return *this;
		}

		const_iterator &operator--()
		{
			if (node->prev == -1)
			{
				if (node->block->prev == nullptr)
				{
				}                                         // ошибочка
				Block< T > *newBlock = node->block->prev; // переход к следующему блоку
				node = newBlock->data[newBlock->last];    // переход к первому элементу в новом блоке
			}
			else
			{
				node = node->block->data[node->prev];
			}
			return *this;
		}

		bool operator==(const const_iterator &other)
		{
			return node->block->id == other.node->block->id && node->id == other.node;
		}

		bool operator!=(const const_iterator &other)
		{
			return !(*this == other);
		}

		bool operator<(const const_iterator &other)
		{
			if (this->node->block->id < other.node->block->id)
			{
				return true;
			}
			if (this->node->block->id == other.node->block->id)
			{
				return this->node->id < other.node->id;
			}
			return false;
		}

		bool operator>(const const_iterator &other)
		{
			if (this->node->block->id > other.node->block->id)
			{
				return true;
			}
			if (this->node->block->id == other.node->block->id)
			{
				return this->node->id > other.node->id;
			}
			return false;
		}

		bool operator<=(const const_iterator &other)
		{
			return !(*this > other);
		}

		bool operator>=(const const_iterator &other)
		{
			return !(*this < other);
		}
	};

	class iterator : public const_iterator
	{
		friend class BucketStorage;

	public:
		T operator*()
		{
			return *(const_iterator::node->data);
		}

		T *operator->()
		{
			return const_iterator::node->data;
		}
	};


	// методы
	iterator insert(const T &value)
	{
		const T *val = &value;
		count++;
		if (globalEmptyStack != nullptr) // есть удалённые
		{
			Node< T > oldNode = globalEmptyStack->data[globalEmptyStack->stack];
			// беру ноду в массиве, где есть свободное место
			globalEmptyStack->stack = oldNode.next; // перехожу по внутреннему стеку
			oldNode.next = -1;                      // убираю из удалнных
			if (globalEmptyStack->stack == block_capaсity)
			// значит внутренний стек стал пустым и надо обнулить его и глобальный стек блоков т.к. этот блок теперь полный
			{
				globalEmptyStack->stack = -1;
				globalEmptyStack = globalEmptyStack->prevEmpty;
			}
			// т.к. этот элемент уже был удалён и теперь снова вставлен в порядке стека, его активные соседи актуальны
			if (oldNode.activePrev > -1)
			{
				globalEmptyStack->data[oldNode.activePrev].activeNext = oldNode.id; // теперь мы для кого-то активные справа
			}
			if (oldNode.activeNext < block_capaсity)
			{
				globalEmptyStack->data[oldNode.activeNext].activePrev = oldNode.id; // теперь мы для кого-то активные слева
			}
			oldNode.data = val; // добавили значение
			oldNode.status = 1; // запомнили что активны
			// надо проверить first и last как локальный, так и глобальный
			if (globalEmptyStack->first == -1 || globalEmptyStack->first > oldNode.id)
			{
				globalEmptyStack->first = oldNode.id;
			}
			if (globalEmptyStack->last == -1 || globalEmptyStack->last < oldNode.id)
			{
				globalEmptyStack->last = oldNode.id;
			}
			// глобальные:
			if (globalFirst == nullptr || globalFirst->block->id > globalEmptyStack->id ||
			    globalFirst->block->id == globalEmptyStack->id && globalFirst->id > oldNode.id)
			{
				globalFirst = &oldNode;
			}
			if (globalLast == nullptr || globalLast->block->id < globalEmptyStack->id ||
			    globalLast->block->id == globalEmptyStack->id && globalLast->id < oldNode.id)
			{
				globalLast = &oldNode;
			}
			oldNode.block->data[oldNode.id].data = val;
			++globalEmptyStack->blockCount;
			return iterator(&oldNode);
		}
		if (myEmpty != block_capaсity) // есть пустые
		{
			lastBlock->data[myEmpty] = Node< T >(val, myEmpty - 1, block_capaсity, myEmpty, &lastBlock, 1);
			//создали ноду
			lastBlock->data[myEmpty - 1].activeNext = myEmpty;
			myEmpty++;
			++lastBlock->blockCount;
			return iterator(&(lastBlock->data[myEmpty - 1]));
		}
		// надо создать блок :(
		Block< T > newBlock = Block< T >(block_capaсity, &lastBlock);
		lastBlock->next = &newBlock; // добавляем его в список блоков
		lastBlock = &newBlock; // перемещаем указатель на последний блок
		newBlock.data[0] = Node< T >(val, -1, block_capaсity, 0, &lastBlock, 1); // создаём первую ноду
		myEmpty = 1;
		return iterator(&(newBlock.data[0]));
	}

	iterator insert(T &&value)
	{
		T* val = new T(std::move(value));
		return insert(*val);
	}

	iterator erase(const_iterator it)
	{
		Node< T > *value = it.node;
		if (value->status == 0)
		{
			//ошибочка
		}
		Block< T > *actualBlock = it.node->block;
		// уберём ноду из списка активных: (при этом значения соседей не убираем, они будут актуальны при вставке)
		if (value->activePrev > -1)
		{
			actualBlock->data[value->activePrev].next = value->activeNext;
		}
		if (value->activeNext < block_capaсity)
		{
			actualBlock->data[value->activeNext].prev = value->activePrev;
		}
		// добавим ноду в локальный стек удалённых:
		value->next = actualBlock->stack;
		actualBlock->stack = value->id;
		if (actualBlock->nextEmpty == nullptr && actualBlock->prevEmpty == nullptr)
		// если этого элемента небыло в стеке блоков с пустыми элементам, то добавим
		{
			if (globalEmptyStack == nullptr)
			{
				globalEmptyStack = actualBlock;
			}
			else
			{
				globalEmptyStack->nextEmpty = actualBlock;
				actualBlock->prevEmpty = globalEmptyStack;
				globalEmptyStack = actualBlock;
			}
		}
		--actualBlock->blockCount;
		count--;



		// НАДО РАЗОБРАТЬСЯ С FIRST, LAST И Т.П.



		bool flag = lastBlock->id == actualBlock->id && value->next == block_capaсity;
		iterator result = iterator(&value); // ??????????
		value->status = 0;
		if (actualBlock->blockCount == 0) // нужно удалить блок
		{
			// разбираемся со списком блоков
			if (actualBlock->prev == nullptr)
			{
				firstBlock = actualBlock->next;
			}
			else
			{
				actualBlock->prev->next = actualBlock->next;
			}
			if (actualBlock->next == nullptr)
			{
				lastBlock = actualBlock->prev;
			}
			else
			{
				actualBlock->next->prev = actualBlock->prev;
			}
			// разбираемся со списком в котором есть пустые элементы
			if (actualBlock == globalEmptyStack)
			{
				globalEmptyStack = actualBlock->prevEmpty;
			}
			if (actualBlock->nextEmpty != nullptr)
			{
				actualBlock->nextEmpty->prevEmpty = actualBlock->prevEmpty;
			}
			if (actualBlock->prevEmpty != nullptr)
			{
				actualBlock->prevEmpty->nextEmpty = actualBlock->nextEmpty;
			}
			delete *actualBlock;
		}
		return flag ? end() : result++;
	}

	bool empty()
	{
		return count == 0;
	}

	T size()
	{
		return count;
	}

	T copacity()
	{
		return (lastBlock->id + 1) * block_capaсity - count;
	}

	void shrink_to_fit()
	{
		BucketStorage result = new BucketStorage(count);
		// ебанутая хуйня, надо продумать
	}

	void clear()
	{
		for (int32_t i = 0; i < lastBlock->id; i++)
		{
			Block< T > *buffer = firstBlock->next;
			delete *firstBlock;
			firstBlock = buffer;
		}
		count = 0;
		globalFirst = nullptr;
		globalLast = nullptr;
		firstBlock = lastBlock = nullptr;
		myEmpty = 0;
	}

	void swap(BucketStorage &other)
	{
		std::swap(count, other.count);
		std::swap(globalFirst, other.globalFirst);
		std::swap(globalLast, other.globalLast);
		std::swap(block_capaсity, other.block_capaсity);
		std::swap(myEmpty, other.myEmpty);
		std::swap(firstBlock, other.firstBlock); // это влечёт за собой свап всех блоков
		std::swap(lastBlock, other.lastBlock);
		std::swap(globalEmptyStack, other.globalEmptyStack);
	}

	// методы для итератора
	iterator begin()
	{
		return iterator(globalFirst);
	}

	/*const_iterator begin()
	{
		return const_iterator(globalFirst);
	}*/

	const_iterator cbegin()
	{
	} // хуй знает что это
	iterator end()
	{
		return iterator(globalLast->next);
	}

	/*const_iterator end()
	{
		return const_iterator(globalLast->next);
	}*/

	const_iterator cend()
	{
		// это тоже хуй знает
	}

	iterator get_to_distance(iterator it, const int32_t distance)
	{
		for (int32_t i = 0; i < distance; i++)
		{
			++it;
		}
		return it;
	}

private:
	int64_t count = 0;                // кол-во заполненных ячейек
	Node< T > *globalFirst = nullptr; // первый и последний не пустой элемент для методов итератора
	Node< T > *globalLast = nullptr;
	int32_t block_capaсity = 64;      // размер блока (у всех блоков одинаковый)
	int32_t myEmpty = block_capaсity; // индекс пустой ячейки после которой мы никогда небыли в последенем блоке
	// (если он больше block_capaсity, значит мы уже были во всех ячейках блока)
	Block< T > *firstBlock = nullptr;       // указатель на первый блок
	Block< T > *lastBlock = nullptr;        // указатель на последний блок, в котором должен быть empty
	Block< T > *globalEmptyStack = nullptr; // указатель на стек пустых блоков
};