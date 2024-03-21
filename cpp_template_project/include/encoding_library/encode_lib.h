#pragma once

#include <array>
#include <string>
#include <vector>

#include "encode_constants.h"

using encoded_bytes = std::vector<unsigned char>;
using tag = unsigned char;
using size_type = int;
using index_type = int;

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

  void writing_string(std::string&);
};

EncodeData unpack(encoded_bytes&);

encoded_bytes pack(EncodeData&);