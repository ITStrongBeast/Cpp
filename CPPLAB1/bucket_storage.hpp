#pragma once

#include "Block.hpp"
#include "Node.hpp"

#include <gtest/internal/gtest-type-util.h>

#include <algorithm>
#include <cstdint>

template<typename T>
class BucketStorage
{
public:
	using value_type = T;
	using reference = value_type&;
	using const_reference = const value_type&;
	using difference_type = std::ptrdiff_t;
	using size_type = std::size_t;

	// конструкторы контейнера
	BucketStorage() = default;

	// Конструктор копирования
	BucketStorage(const BucketStorage& other) {
		//std::copy(other.begin(), other.end(), data);
	}

	// Оператор копирующего присваивания
	BucketStorage& operator=(const BucketStorage& other) {
		if (this != &other) {
			BucketStorage temp(other); // Копируем, затем меняем местами
			swap(temp);
		}
		return *this;
	}

	// Конструктор перемещения
	BucketStorage(BucketStorage&& other) noexcept {
		swap(other);
	}

	// Оператор перемещающего присваивания
	BucketStorage& operator=(BucketStorage&& other) noexcept {
		if (this != &other) {
			BucketStorage temp(std::move(other)); // Перемещаем, затем меняем местами
			swap(temp);
		}
		return *this;
	}

	BucketStorage(value_type block_capaсity)
	{
		this->block_capaсity = block_capaсity;
	}

	~BucketStorage() = default;

	// итератор контейнера
	template<typename C>
	class abstract_iterator
	{
		friend class BucketStorage;
		Node< value_type > *node = nullptr;

	public:
		abstract_iterator(Node< value_type > *node)
		{
			this->node = node;
		}

		abstract_iterator &operator=(const abstract_iterator &other)
		{
			this->node = other.node;
			return *this;
		}

		abstract_iterator &operator++()
		{
			if (node->next == node->block->capaсity)
			{
				if (node->block->next == nullptr)
				{
					// ошибочка
				}
				Block< value_type > *newBlock = node->block->next; // переход к следующему блоку
				node = newBlock->data[newBlock->first];   // переход к первому элементу в новом блоке
			}
			else
			{
				node = node->block->data[node->next];
			}
			return *this;
		}

		abstract_iterator &operator--()
		{
			if (node->prev == -1)
			{
				if (node->block->prev == nullptr)
				{
				}                                         // ошибочка
				Block< value_type > *newBlock = node->block->prev; // переход к следующему блоку
				node = newBlock->data[newBlock->last];    // переход к первому элементу в новом блоке
			}
			else
			{
				node = node->block->data[node->prev];
			}
			return *this;
		}

		bool operator==(const abstract_iterator &other)
		{
			return node->block->id == other.node->block->id && node->id == other.node;
		}

		bool operator!=(const abstract_iterator &other)
		{
			return !(*this == other);
		}

		bool operator<(const abstract_iterator &other)
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

		bool operator>(const abstract_iterator &other)
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

		bool operator<=(const abstract_iterator &other)
		{
			return !(*this > other);
		}

		bool operator>=(const abstract_iterator &other)
		{
			return !(*this < other);
		}
	};

	class iterator : public abstract_iterator
	{
		friend class BucketStorage;

	public:
		value_type operator*()
		{
			return *(abstract_iterator::node->data);
		}

		value_type *operator->()
		{
			return abstract_iterator::node->data;
		}
	};

	class const_iterator : public abstract_iterator
	{
		friend class BucketStorage;

	public:
		value_type operator*() const
		{
			return *(abstract_iterator::node->data);
		}

		value_type *operator->() const
		{
			return abstract_iterator::node->data;
		}
	};


	using iterator = abstract_iterator
	using const_iterator = const value_type*;


	// методы
	iterator insert(const value_type &value)
	{
		const value_type *val = &value;
		count++;
		if (globalEmptyStack != nullptr) // есть удалённые
		{
			Node< value_type > oldNode = globalEmptyStack->data[globalEmptyStack->stack];
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
			lastBlock->data[myEmpty] = Node< value_type >(val, myEmpty - 1, block_capaсity, myEmpty, &lastBlock, 1);
			//создали ноду
			lastBlock->data[myEmpty - 1].activeNext = myEmpty;
			myEmpty++;
			++lastBlock->blockCount;
			return iterator(&(lastBlock->data[myEmpty - 1]));
		}
		// надо создать блок :(
		Block< value_type > newBlock = Block< value_type >(block_capaсity, &lastBlock);
		lastBlock->next = &newBlock; // добавляем его в список блоков
		lastBlock = &newBlock; // перемещаем указатель на последний блок
		newBlock.data[0] = Node< value_type >(val, -1, block_capaсity, 0, &lastBlock, 1); // создаём первую ноду
		myEmpty = 1;
		return iterator(&(newBlock.data[0]));
	}

	iterator insert(value_type &&value)
	{
		value_type* val = new value_type(std::move(value));
		return insert(*val);
	}

	iterator erase(const_iterator it)
	{
		Node< value_type > *value = it.node;
		if (value->status == 0)
		{
			//ошибочка
		}
		Block< value_type > *actualBlock = it.node->block;
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

	bool empty() noexcept;
	value_type size() noexcept;
	value_type copacity() noexcept;
	void shrink_to_fit() noexcept;
	void clear() noexcept;
	void swap(BucketStorage &other) noexcept;

	// методы для итератора
	iterator begin()
	{
		return iterator(globalFirst);
	}

	/*const_iterator begin() noexcept
	{
		return const_iterator(globalFirst);
	}*/

	const_iterator cbegin() noexcept
	{
	} // хуй знает что это

	iterator end() noexcept
	{
		return iterator(globalLast->next);
	}

	/*const_iterator end() noexcept
	{
		return const_iterator(globalLast->next);
	}*/

	const_iterator cend() noexcept
	{
		// это тоже хуй знает
	}

	iterator get_to_distance(iterator it, const difference_type distance) noexcept
	{
		for (int32_t i = 0; i < distance; i++)
		{
			++it;
		}
		return it;
	}

private:
	int64_t count = 0;                // кол-во заполненных ячейек
	Node< value_type > *globalFirst = nullptr; // первый и последний не пустой элемент для методов итератора
	Node< value_type > *globalLast = nullptr;
	int32_t block_capaсity = 64;      // размер блока (у всех блоков одинаковый)
	int32_t myEmpty = block_capaсity; // индекс пустой ячейки после которой мы никогда небыли в последенем блоке
	// (если он больше block_capaсity, значит мы уже были во всех ячейках блока)
	Block< value_type > *firstBlock = nullptr;       // указатель на первый блок
	Block< value_type > *lastBlock = nullptr;        // указатель на последний блок, в котором должен быть empty
	Block< value_type > *globalEmptyStack = nullptr; // указатель на стек пустых блоков
};


template <typename T>
bool BucketStorage<T>::empty() noexcept
{
	return count == 0;
}

template <typename T>
T BucketStorage<T>::size() noexcept
{
	return count;
}

template <typename T>
T BucketStorage<T>::copacity() noexcept
{
	return (lastBlock->id + 1) * block_capaсity - count;
}

template <typename T>
void BucketStorage<T>::swap(BucketStorage &other) noexcept
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

template <typename T>
void BucketStorage<T>::clear() noexcept
{
	for (int32_t i = 0; i < lastBlock->id; i++)
	{
		Block< value_type > *buffer = firstBlock->next;
		delete *firstBlock;
		firstBlock = buffer;
	}
	count = 0;
	globalFirst = nullptr;
	globalLast = nullptr;
	firstBlock = lastBlock = nullptr;
	myEmpty = 0;
}

template <typename T>
void BucketStorage<T>::shrink_to_fit() noexcept
{
	// ебанутая хуйня, надо продумать
}
