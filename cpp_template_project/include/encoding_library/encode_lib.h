#pragma once

#include <array>
#include <string>
#include <vector>

using encoded_bytes = std::vector<unsigned char>;
using tag = unsigned char;
using size_type = int;
using index_type = int;

constexpr int eighth_bit = 128;
constexpr int seventh_bit = 64;
constexpr int sixth_bit = 32;
constexpr int fifth_bit = 16;
constexpr int fourth_bit = 8;
constexpr int third_bit = 4;
constexpr int second_bit = 2;
constexpr int first_bit = 1;
constexpr unsigned char bool_true_tag = 32;
constexpr unsigned char bool_false_tag = 64;
constexpr unsigned char start_end_tag = 0;
constexpr int bits = 8;
constexpr int bits_for_size = 4;
constexpr unsigned char undefined_type_tag = 20;
constexpr int first_six_bytes = 6;

struct EncodeData {
  encoded_bytes data;
  std::string key;
  std::string vec;
  std::string algorithm_name;
  bool encode;
};

class Serializing {
 public:
  void set_bytes(encoded_bytes byte_array) { bytes = byte_array; }
  void set_object(EncodeData en_object) { object = en_object; }
  encoded_bytes serialize_object_to_bytes();
  EncodeData serialize_bytes_to_object();

 private:
  EncodeData object;
  encoded_bytes bytes;

  int counting_size_from_bytes(index_type);

  int counting_size_from_tag(tag);

  std::string making_string(index_type&, size_type&);

  bool translate_bool_tag(index_type);

  void counting_size_to_bytes(size_type);

  void writing_string_tag(size_type);

  void writing_string(std::string);
};

EncodeData unpack(encoded_bytes);

encoded_bytes pack(EncodeData);