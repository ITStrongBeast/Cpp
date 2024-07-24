#pragma once

#include <cstddef>
#include <iterator>
#include <stdexcept>

template< typename T >
class BucketStorage
{
  public:
	class Iterator;
	class ConstIterator;
	class Node;
	class Block;

	using value_type = T;
	using reference = T&;
	using const_reference = const T&;
	using difference_type = std::ptrdiff_t;
	using size_type = std::size_t;
	using iterator = Iterator;
	using const_iterator = Iterator;

	BucketStorage() noexcept;
	BucketStorage(const BucketStorage& other);
	BucketStorage(BucketStorage&& other) noexcept;
	explicit BucketStorage(size_type block_capacity);
	~BucketStorage();
	BucketStorage& operator=(const BucketStorage& other) noexcept;
	BucketStorage& operator=(BucketStorage&& other) noexcept;
	iterator insert(const value_type& value);
	iterator insert(value_type&& value);
	iterator myInsert(value_type* value);
	iterator erase(const_iterator it);
	[[nodiscard]] bool empty() const noexcept;
	[[nodiscard]] size_type size() const noexcept;
	[[nodiscard]] size_type capacity() const noexcept;
	void shrink_to_fit();
	void clear();
	void swap(BucketStorage& other) noexcept;
	Node* myBegin() const noexcept;
	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	const_iterator cbegin() noexcept;
	iterator end() noexcept;
	const_iterator end() const noexcept;
	const_iterator cend() noexcept;
	static iterator get_to_distance(iterator it, difference_type distance);

	class Iterator
	{
	  public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		Node* value;

		Iterator();
		explicit Iterator(Node* value);
		~Iterator();
		Iterator operator++();
		Iterator operator++(int);
		Iterator operator--();
		Iterator operator--(int);
		bool operator==(const Iterator& other) const;
		bool operator!=(const Iterator& other) const;
		bool operator<(const Iterator& other) const;
		bool operator>(const Iterator& other) const;
		bool operator<=(const Iterator& other) const;
		bool operator>=(const Iterator& other) const;
		BucketStorage::value_type* operator->();
		reference operator*();
	};

	class ConstIterator : public Iterator
	{
	  public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = const T;
		using difference_type = std::ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;

		ConstIterator() : Iterator() {}
		explicit ConstIterator(const Node* value) : Iterator(const_cast< Node* >(value)) {}

		const BucketStorage::value_type* operator->() const { return Iterator::operator->(); }
		reference operator*() const { return *Iterator::operator->(); }
	};

	class Node
	{
	  public:
		value_type* value = nullptr;
		size_type id = 0;
		Block* block = nullptr;
		BucketStorage< T >* bucket = nullptr;

		Node() = default;
		~Node() { value = nullptr; }
	};

	class Block
	{
	  public:
		Block* prev = nullptr;
		Block* next = nullptr;
		Node* data = nullptr;
		size_type id = 0;
		size_type count = 0;
		Block() = default;
		~Block() { delete[] this->data; }
	};

	Block* head = nullptr;
	Block* tail;
	size_type block_capacity = 64;
	size_type count = 0;
};

template< typename T >
BucketStorage< T >::BucketStorage() noexcept
{
	tail = new Block();
	tail->id = 1;
	tail->data = new Node[1];
	tail->data[0].block = tail;
	tail->data[0].bucket = this;
}

template< typename T >
BucketStorage< T >::BucketStorage(const BucketStorage& other) : block_capacity(other.block_capacity)
{
	tail = new Block();
	tail->id = 1;
	tail->data = new Node[1];
	tail->data[0].block = tail;
	tail->data[0].bucket = this;

	for (const auto& value : other)
	{
		insert(value);
	}
}

template< typename T >
BucketStorage< T >::BucketStorage(BucketStorage&& other) noexcept :
	block_capacity(other.block_capacity), head(other.head), tail(other.tail), count(other.count)
{
	other.head = nullptr;
	other.tail = nullptr;
	other.count = 0;
	other.block_capacity = 0;
}

template< typename T >
BucketStorage< T >::BucketStorage(size_type block_capacity)
{
	tail = new Block();
	tail->id = 1;
	tail->data = new Node[1];
	tail->data[0].block = tail;
	tail->data[0].bucket = this;

	this->block_capacity = block_capacity;
	this->count = 0;
}

template< typename T >
BucketStorage< T >::~BucketStorage()
{
	while (head != nullptr)
	{
		Block* temp = head;
		head = head->next;
		delete temp;
	}
}

template< typename T >
BucketStorage< T >& BucketStorage< T >::operator=(const BucketStorage& other) noexcept
{
	if (this != &other)
	{
		BucketStorage temp(other);
		this->swap(temp);
	}
	return *this;
}

template< typename T >
BucketStorage< T >& BucketStorage< T >::operator=(BucketStorage&& other) noexcept
{
	if (this != &other)
	{
		clear();
		this->head = other.head;
		this->tail = other.tail;
		this->block_capacity = other.block_capacity;
		this->count = other.count;
		other.head = nullptr;
		other.tail = nullptr;
		other.block_capacity = 0;
		other.count = 0;
	}
	return *this;
}

template< typename T >
typename BucketStorage< T >::iterator BucketStorage< T >::myInsert(value_type* value)
{
	Block* buf = head;
	while (buf != nullptr)
	{
		if (buf->count < block_capacity)
		{
			break;
		}
		buf = buf->next;
	}
	if (buf == nullptr || buf->id == tail->id)
	{
		buf = new Block();
		buf->next = tail;
		buf->prev = tail->prev;
		tail->prev = buf;
		if (buf->prev != nullptr)
		{
			buf->prev->next = buf;
		}
		buf->id = tail->id;
		++tail->id;
		buf->data = new Node[block_capacity];
		if (head == nullptr)
		{
			head = buf;
		}
	}
	++buf->count;
	++this->count;
	for (size_type i = 0; i < this->block_capacity; i++)
	{
		if (buf->data[i].value == nullptr)
		{
			buf->data[i].value = value;
			buf->data[i].id = i;
			buf->data[i].bucket = this;
			buf->data[i].block = buf;
			return iterator(&buf->data[i]);
		}
	}
	return iterator(&buf->data[0]);
}

template< typename T >
typename BucketStorage< T >::iterator BucketStorage< T >::insert(const value_type& value)
{
	auto* newValue = new value_type(value);
	return myInsert(newValue);
}

template< typename T >
typename BucketStorage< T >::iterator BucketStorage< T >::insert(value_type&& value)
{
	auto* newValue = new value_type(std::move(value));
	return myInsert(newValue);
}

template< typename T >
typename BucketStorage< T >::iterator BucketStorage< T >::erase(const_iterator it)
{
	Block* block = it.value->block;
	if (block->id == tail->id)
	{
		throw std::runtime_error("Iterator is out of range");
	}
	--block->count;
	--this->count;
	it.value->value = nullptr;
	if (block->count == 0)
	{
		if (block->prev != nullptr)
		{
			block->prev->next = block->next;
		}
		if (block->next != nullptr)
		{
			block->next->prev = block->prev;
		}
		if (block->id == head->id)
		{
			head = block->next;
		}
		delete block;
	}
	it++;
	return it;
}

template< typename T >
bool BucketStorage< T >::empty() const noexcept
{
	return this->count == 0;
}

template< typename T >
typename BucketStorage< T >::size_type BucketStorage< T >::size() const noexcept
{
	return this->count;
}

template< typename T >
typename BucketStorage< T >::size_type BucketStorage< T >::capacity() const noexcept
{
	size_type activ = 0;
	Block* buf = head;
	if (buf == nullptr)
	{
		return 0;
	}
	while (buf != nullptr)
	{
		activ++;
		buf = buf->next;
	}
	activ--;
	return this->block_capacity * activ;
}

template< typename T >
void BucketStorage< T >::shrink_to_fit()
{
	auto* newBlock = new Block();
	newBlock->id = 1;
	newBlock->data = new Node[this->count];
	newBlock->count = this->count;
	iterator it = begin();
	for (size_type i = 0; i < this->count; i++)
	{
		newBlock->data[i].value = it.value->value;
		newBlock->data[i].id = i;
		newBlock->data[i].block = newBlock;
		newBlock->data[i].bucket = this;
		++it;
	}
	head = newBlock;
	tail->prev = newBlock;
	newBlock->next = tail;
}

template< typename T >
void BucketStorage< T >::clear()
{
	Block* buf = head;
	while (buf != nullptr && buf->next != nullptr)
	{
		Block* temp = buf;
		buf = buf->next;
		delete temp;
	}
	this->block_capacity = 64;
	this->count = 0;
}

template< typename T >
void BucketStorage< T >::swap(BucketStorage& other) noexcept
{
	std::swap(this->head, other.head);
	std::swap(this->tail, other.tail);
	std::swap(this->block_capacity, other.block_capacity);
	std::swap(this->count, other.count);
}

template< typename T >
typename BucketStorage< T >::Node* BucketStorage< T >::myBegin() const noexcept
{
	Block* buf = head;
	if (buf == nullptr)
	{
		return &(tail->data[0]);
	}
	for (size_type i = 0; i < this->block_capacity; i++)
	{
		if (buf->data[i].value != nullptr)
		{
			return &(buf->data[i]);
		}
	}
	return &(tail->data[0]);
}

template< typename T >
typename BucketStorage< T >::iterator BucketStorage< T >::begin() noexcept
{
	return iterator(myBegin());
}

template< typename T >
typename BucketStorage< T >::const_iterator BucketStorage< T >::begin() const noexcept
{
	return const_iterator(myBegin());
}

template< typename T >
typename BucketStorage< T >::const_iterator BucketStorage< T >::cbegin() noexcept
{
	return const_iterator(myBegin());
}

template< typename T >
typename BucketStorage< T >::iterator BucketStorage< T >::end() noexcept
{
	return iterator(&tail->data[0]);
}

template< typename T >
typename BucketStorage< T >::const_iterator BucketStorage< T >::end() const noexcept
{
	return const_iterator(&tail->data[0]);
}

template< typename T >
typename BucketStorage< T >::const_iterator BucketStorage< T >::cend() noexcept
{
	return const_iterator(&tail->data[0]);
}

template< typename T >
typename BucketStorage< T >::iterator BucketStorage< T >::get_to_distance(iterator it, difference_type distance)
{
	for (difference_type i = 0; i < distance; ++i)
	{
		++it;
	}
	return it;
}

template< typename T >
BucketStorage< T >::Iterator::Iterator() = default;

template< typename T >
BucketStorage< T >::Iterator::Iterator(Node* value) : value(value)
{
}

template< typename T >
BucketStorage< T >::Iterator::~Iterator() = default;

template< typename T >
typename BucketStorage< T >::iterator BucketStorage< T >::Iterator::operator++()
{
	this->operator++(0);
	return *this;
}

template< typename T >
typename BucketStorage< T >::iterator BucketStorage< T >::Iterator::operator++(int)
{
	Block* buf = value->block;
	if (buf->id == value->bucket->tail->id)
	{
		throw std::runtime_error("Iterator is out of range");
	}
	for (size_type i = value->id + 1; i < value->bucket->block_capacity; i++)
	{
		if (buf->data[i].value != nullptr)
		{
			value = &buf->data[i];
			return *this;
		}
	}
	buf = buf->next;
	if (buf->id == value->bucket->tail->id)
	{
		value = &buf->data[0];
		return *this;
	}
	for (size_type i = 0; i < value->bucket->block_capacity; i++)
	{
		if (buf->data[i].value != nullptr)
		{
			value = &buf->data[i];
			return *this;
		}
	}
	return *this;
}

template< typename T >
typename BucketStorage< T >::iterator BucketStorage< T >::Iterator::operator--()
{
	this->operator--(0);
	return *this;
}

template< typename T >
typename BucketStorage< T >::iterator BucketStorage< T >::Iterator::operator--(int)
{
	Block* buf = value->block;
	if (buf->id == value->bucket->head->id)
	{
		throw std::runtime_error("Iterator is out of range");
	}
	for (size_type i = value->id - 1; i > -1; --i)
	{
		if (buf->data[i].value != nullptr)
		{
			value = &buf->data[i];
			return *this;
		}
	}
	buf = buf->prev;
	for (size_type i = value->id - 1; i > -1; --i)
	{
		if (buf->data[i].value != nullptr)
		{
			value = &buf->data[i];
			return *this;
		}
	}
	return *this;
}

template< typename T >
bool BucketStorage< T >::Iterator::operator==(const Iterator& other) const
{
	return this->value->block->id == other.value->block->id && this->value->id == other.value->id;
}

template< typename T >
bool BucketStorage< T >::Iterator::operator!=(const Iterator& other) const
{
	return !(*this == other);
}

template< typename T >
bool BucketStorage< T >::Iterator::operator<(const Iterator& other) const
{
	if (this->value->block->id < other.value->block->id)
	{
		return true;
	}
	if (this->value->block->id > other.value->block->id)
	{
		return false;
	}
	return this->value->id < other.value->id;
}

template< typename T >
bool BucketStorage< T >::Iterator::operator>(const Iterator& other) const
{
	return other < *this;
}

template< typename T >
bool BucketStorage< T >::Iterator::operator<=(const Iterator& other) const
{
	return !(*this > other);
}

template< typename T >
bool BucketStorage< T >::Iterator::operator>=(const Iterator& other) const
{
	return !(*this < other);
}

template< typename T >
typename BucketStorage< T >::value_type* BucketStorage< T >::Iterator::operator->()
{
	return this->value->value;
}

template< typename T >
typename BucketStorage< T >::reference BucketStorage< T >::Iterator::operator*()
{
	return *(this->value->value);
}
