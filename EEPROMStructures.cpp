#include <EEPROM.h>
#include <stdlib.h>

#include "EEPROMStructures.h"

template<typename T>
EEPROMValue<T>::EEPROMValue(size_t memory_pos) {
  EEPROM.get(memory_pos, value);
}

template<typename T>
T EEPROMValue<T>::get() const {
  return value;
}

template<typename T>
void EEPROMValue<T>::put(const T& value) {
  this->value = value;
  EEPROM.put(memory_pos, value);
}

template class EEPROMValue<uint8_t>;

template<typename T>
EEPROMQueue<T>::EEPROMQueue(size_t max_size, size_t memory_start, T end_value) {
  max_size++;
  this->array = calloc(max_size, sizeof(*this->array));
  this->max_size = max_size;
  this->memory_start = memory_start;
  this->end_value = end_value;
  readQueue();
}

template<typename T>
void EEPROMQueue<T>::readQueue() {
  head = -1;
  tail = -1;

  T temp_value;
  bool old = (EEPROM.get(memory_start + (max_size - 1) * sizeof(*array), temp_value) == end_value);
  for (size_t i = 0; i < max_size; i++) {
    bool cur = EEPROM.get(memory_start + i * sizeof(*array), temp_value) == end_value;
    if (cur) {
      if (!old) {
        head = (i == 0 ? max_size - 1 : i - 1);
        if (tail != -1) {
          break;
        }
      }
    } else {
      if (old) {
        tail = i;
        if (head != -1) {
          break;
        }
      }
    }
    old = cur;
  }

  if (tail == -1 || head == -1) {
    head = max_size - 1;
    tail = 0;
  } else {
    size_t i = tail;
    while (true) {
      EEPROM.get(memory_start + i * sizeof(*array), array[i]);

      if (i == head) {
        break;
      }
      i = i + 1 == max_size ? 0 : i + 1;
    }
  }
}

#include <Arduino.h>

template<typename T>
void EEPROMQueue<T>::push(const T& value) {
  head = (head == max_size - 1 ? 0 : head + 1);
  array[head] = value;
  EEPROM.put(memory_start + head * sizeof(*array), array[head]);
}

template<typename T>
T EEPROMQueue<T>::pop() {
  T result = array[tail];
  EEPROM.put(memory_start + tail * sizeof(*array), end_value);
  tail = (tail + 1 == max_size ? 0 : tail + 1);
  return result;
}

template<typename T>
T EEPROMQueue<T>::operator[](size_t idx) const {
  return array[tail + idx < max_size ? tail + idx : tail + idx - max_size];
}
template<typename T>
size_t EEPROMQueue<T>::size() const {
  if ((head + 1 == max_size ? 0 : head + 1) == tail) {
    return 0;
  }
  return tail <= head ? head - tail + 1 : head + max_size - tail + 1;
}

template<typename T>
T* EEPROMQueue<T>::getArray() const {
  return array;
}

template<typename T>
void EEPROMQueue<T>::clearMemory() {
  for (size_t i = 0; i < max_size; i++) {
    EEPROM.put(memory_start + i * sizeof(*array), end_value);
  }
  head = max_size - 1;
  tail = 0;
}

template<typename T>
EEPROMQueue<T>::~EEPROMQueue() {
  free(array);
}

template class EEPROMQueue<uint16_t>;
