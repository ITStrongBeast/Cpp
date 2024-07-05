#pragma once

#include <cstdint>

template<typename T>
class Block;

template<typename T>
class Node
{
public:
	int8_t status;      // 0 - empty, 1 - filled
	int32_t id;         // по сути индекс в блоке
	Block< T > *block;  // ссылка на блок
	int32_t next = -1;  // индекс следующего неактивного элемента
	int32_t activeNext; // индекс следующего и предыдущего активного элемента
	int32_t activePrev;
	const T *data; // ссылка на данные
	Node() = default;
	Node(const T *data, int32_t activePrev, int32_t activeNext, int32_t id, Block< T > **block, int8_t status)
		: data(data), activePrev(activePrev), activeNext(activeNext), id(id), block(*block), status(status)
	{
	}

	~Node() = default;
};
