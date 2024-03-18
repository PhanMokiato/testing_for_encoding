#include <cmath>
#include <iostream>
#include <vector>

#include "encode_lib.h"

int main() {
  EncodeData object;
  object.data.push_back(3);
  object.data.push_back(4);
  object.data.push_back(56);
  object.key = "key";
  object.vec = "vec";
  object.algorithm_name = "Algorithm name for encoding";
  object.encode = false;
  std::vector<unsigned char> array = pack(object);
  for (int i = 0; i < array.size(); ++i) {
    std::cout << i + 1 << "). ";
    for (int k = 128; k > 0; k = k / 2) {
      if (array[i] & k) {
        std::cout << "1 ";
      } else {
        std::cout << "0 ";
      }
    }
    std::cout << std::endl;
  }
  EncodeData object_copy = unpack(array.data());
  for (auto elem : object_copy.data) {
    std::cout << static_cast<int>(elem) << " ";
  }
  std::cout << std::endl;
  std::cout << object_copy.key << std::endl;
  std::cout << object_copy.vec << std::endl;
  std::cout << object_copy.algorithm_name << std::endl;
  std::cout << object_copy.encode << std::endl;
  return 0;
}