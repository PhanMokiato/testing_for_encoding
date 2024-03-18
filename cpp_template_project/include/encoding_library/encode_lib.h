#pragma once

#include <array>
#include <string>
#include <vector>

struct EncodeData {
  std::vector<unsigned char> data;
  std::string key;
  std::string vec;
  std::string algorithm_name;
  bool encode;
};

int counting_size_from_bytes(unsigned char array[], int);

int counting_size_from_tag(unsigned char);

void making_string(unsigned char array[], int&, int&, std::string&);

EncodeData unpack(unsigned char array[]);

void counting_size_to_bytes(std::vector<unsigned char>& array, int);

void string_tag(std::vector<unsigned char>& array, int);

void writing_string(std::vector<unsigned char>& array, std::string);

std::vector<unsigned char> pack(EncodeData);