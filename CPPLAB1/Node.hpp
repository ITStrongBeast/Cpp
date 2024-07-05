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
	T *data; // данные
	Node() = default;
	Node(T data, int32_t activePrev, int32_t activeNext, int32_t id, Block< T > **block, int8_t status)
		: data(data), activePrev(activePrev), activeNext(activeNext), id(id), block(*block), status(status)
	{
	}

	~Node() = default;
};


#pragma once

#include <cstddef>
#include <stdexcept>

// INTERFACE

template< typename T >
class BucketStorage
{
  public:
  struct BSIterator;

  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using difference_type = std::ptrdiff_t;
  using size_type = size_t;
  using iterator = BSIterator;
  using const_iterator = BSIterator;

  struct BSIterator
  {
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Node*;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    BSIterator();
    BSIterator(const BSIterator& other);
    BSIterator(Block block, Node* element);
    ~BSIterator();
    BSIterator& operator=(const BSIterator& other);
    BSIterator operator++(int);
    BSIterator& operator++();
    BSIterator operator--(int);
    BSIterator& operator--();
    bool operator<(const BSIterator& other) const;
    bool operator>(const BSIterator& other) const;
    bool operator<=(const BSIterator& other) const;
    bool operator>=(const BSIterator& other) const;
    bool operator==(const BSIterator& other) const;
    bool operator!=(const BSIterator& other) const;
    BucketStorage::value_type* operator->();
    reference operator*();
  };

  BucketStorage();
  BucketStorage(const BucketStorage& other);
  BucketStorage(BucketStorage&& other) noexcept;
  explicit BucketStorage(size_type block_capacity);
  ~BucketStorage();
  BucketStorage& operator=(const BucketStorage& other);
  iterator insert(const value_type& value);
  iterator insert(value_type&& value);
  iterator erase(const_iterator it);
  [[nodiscard]] bool empty() const noexcept;
  [[nodiscard]] size_type size() const noexcept;
  [[nodiscard]] size_type capacity() const noexcept;
  void shrink_to_fit();
  void clear();
  void swap(BucketStorage& other) noexcept;
  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() noexcept;
  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() noexcept;
  static iterator get_to_distance(iterator it, difference_type distance);
};