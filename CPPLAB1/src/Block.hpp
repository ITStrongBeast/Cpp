#pragma once

#include <cstdint>

#include "Node.hpp"

template<typename T>
class BucketStorage;

template<typename T>
class Block
{
	friend class BucketStorage<T>;

	int32_t id = 0;
	int32_t capaсity;       // размер блока
	int32_t blockCount = 1; // кол-во заполненных элементов в блоке
	Block *next = nullptr;  // список блоков
	Block *prev;
	Block *nextEmpty = nullptr; // очередь блоков в которых есть хотя-бы одна пустая ячейка
	Block *prevEmpty = nullptr;
	int32_t first = -1;
	// первый и последний не пустой элемент в блоке (для быстрого нахождении элемента при переходе через блок)
	int32_t last = -1;
	Node< T > *data;    // данные в блоке
	int32_t stack = -1; // индекс массива на голову стека для хранения пустых ячейек
public:
	Block(int32_t block_capacity, Block **lastBlock) : capaсity(block_capacity)
	{
		data = (Node< T > *) malloc(sizeof(Node< T >) * capaсity);
		if (lastBlock != nullptr)
		{
			id = (*lastBlock)->id + 1;
		}
		prev = *lastBlock;
	}

	~Block()
	{
		free(data);
	}
};
