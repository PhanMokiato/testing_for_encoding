#include "encode_lib.h"

#include <cmath>
#include <string>
#include <vector>

int counting_size_from_bytes(unsigned char array[], int index) {
  int data_size = 0;
  int size = index - 3;
  for (int i = 0; index >= size; --index, ++i) {
    for (int k = 128; k > 0; k = k / 2) {
      if (array[index] & k) {
        data_size += k * pow(2, 8 * i);
      }
    }
  }
  return data_size;
}

int counting_size_from_tag(unsigned char size) {
  int data_size = 0;
  for (int k = 8; k > 0; k = k / 2) {
    if (size & k) {
      data_size += k;
    }
  }
  return data_size;
}

void making_string(unsigned char array[], int& start_index, int& data_size,
                   std::string& name) {
  if (array[start_index + data_size] & 16) {
    start_index = start_index + data_size + 5;
    data_size = counting_size_from_bytes(array, start_index - 1);
  } else {
    start_index = start_index + data_size + 1;
    data_size = counting_size_from_tag(array[start_index - 1]);
  }
  for (int i = start_index; i < start_index + data_size; ++i) {
    std::string elem(1, static_cast<char>(array[i]));
    name += elem;
  }
}

EncodeData unpack(unsigned char array[]) {
  EncodeData object;
  int start_index = 6;
  int data_size = counting_size_from_bytes(array, start_index - 1);
  for (int i = start_index; i < start_index + data_size; ++i) {
    object.data.push_back(array[i]);
  }
  making_string(array, start_index, data_size, object.key);
  making_string(array, start_index, data_size, object.vec);
  making_string(array, start_index, data_size, object.algorithm_name);
  object.encode = (array[start_index + data_size] == 32);
  return object;
}

void counting_size_to_bytes(std::vector<unsigned char>& array, int data_size) {
  for (int i = 3; i >= 0; --i) {
    unsigned char elem = 0;
    for (int k = pow(2, 8 * (i + 1) - 1); k > 0; k = k / 2) {
      if (data_size & k) {
        elem += k / pow(2, 8 * i);
      }
    }
    array.push_back(elem);
  }
}

void string_tag(std::vector<unsigned char>& array, int data_size) {
  unsigned char tag = 64 + 32;
  if (data_size >= 16) {
    tag += 16 + 4;
    array.push_back(tag);
    counting_size_to_bytes(array, data_size);
  } else {
    tag += data_size;
    array.push_back(tag);
  }
}

void writing_string(std::vector<unsigned char>& array, std::string name) {
  string_tag(array, name.length());
  for (int i = 0; i < name.length(); ++i) {
    unsigned char elem = static_cast<unsigned char>(name[i]);
    array.push_back(elem);
  }
}

std::vector<unsigned char> pack(EncodeData object) {
  std::vector<unsigned char> data_in_bytes;
  unsigned char start_end_tag(0);
  unsigned char tag(20);
  data_in_bytes.push_back(start_end_tag);
  data_in_bytes.push_back(tag);
  int data_size = object.data.size();
  counting_size_to_bytes(data_in_bytes, data_size);
  data_in_bytes.insert(data_in_bytes.end(), object.data.begin(),
                       object.data.end());
  writing_string(data_in_bytes, object.key);
  writing_string(data_in_bytes, object.vec);
  writing_string(data_in_bytes, object.algorithm_name);
  if (object.encode) {
    tag = 32;
  } else {
    tag = 64;
  }
  data_in_bytes.push_back(tag);
  data_in_bytes.push_back(start_end_tag);
  return data_in_bytes;
}
