// #include <EEPROM.h>

// #include "EEPROMManager.h"

// template<typename T>
// size_t readQueue(T* array, size_t n, size_t queue_start, const T& end_value) {
//   size_t end = -1, start = -1;
//   T temp_value;
//   for (size_t i = 0; i < n; i++) {
//     if (EEPROM.get(queue_start + i * sizeof(*array), temp_value) == end_value) {
//       if (end == -1) {
//         end = i;
//       }
//     } else {
//       if (end != -1 && start == -1) {
//         start = i;
//         break;
//       }
//     }
//   }

//   size_t array_pos = 0;
//   for (size_t i = start; i != end; i = (i + 1 != n ? i + 1 : start)) {
//     EEPROM.get(queue_start + i * sizeof(*array), array[array_pos++]);
//   }

//   return array_pos;
// }
