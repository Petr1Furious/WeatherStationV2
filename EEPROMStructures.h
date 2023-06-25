#pragma once

template<typename T>
class EEPROMValue {
public:
  EEPROMValue(size_t memory_pos);

  T get() const;

  void put(const T& value);

private:
  size_t memory_pos;

  T value;
};

template<typename T>
class EEPROMQueue {
public:
  EEPROMQueue(size_t max_size, size_t memory_start, T end_value);

  void readQueue();

  void push(const T& value);

  T pop();

  T operator[](size_t idx) const;

  size_t size() const;

  T* getArray() const;

  void clearMemory();

  ~EEPROMQueue();

private:
  T* array;

  size_t max_size;

  size_t memory_start;

  T end_value;

  size_t head = -1;

  size_t tail = -1;
};
