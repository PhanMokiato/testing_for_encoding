#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "encode_lib.h"

TEST(working_with_encode_data, pack) {
  EncodeData object;
  object.data.push_back(3);
  object.data.push_back(4);
  object.data.push_back(56);
  object.key = "key";
  object.vec = "vec";
  object.algorithm_name = "Algorithm name for encoding";
  object.encode = false;
  encoded_bytes our_bytes = pack(object);
  EXPECT_EQ(51, our_bytes.size());
  EXPECT_EQ(0, our_bytes[0]);
  EXPECT_EQ(0, our_bytes.back());
  EXPECT_EQ(20, our_bytes[1]);
  EXPECT_EQ(seventh_bit + sixth_bit + second_bit + first_bit, our_bytes[9]);
  EXPECT_EQ(bool_false_tag, our_bytes[49]);
}

TEST(working_with_encode_data, unpack) {
  EncodeData object;
  object.data.push_back(3);
  object.data.push_back(4);
  object.data.push_back(56);
  object.key = "key";
  object.vec = "vec";
  object.algorithm_name = "Algorithm name for encoding";
  object.encode = false;
  encoded_bytes our_bytes = pack(object);
  EncodeData object_copy = unpack(our_bytes);
  EXPECT_EQ(3, object_copy.data.size());
  EXPECT_EQ(3, object_copy.data[0]);
  EXPECT_EQ(4, object_copy.data[1]);
  EXPECT_EQ(56, object_copy.data[2]);
  EXPECT_EQ("key", object_copy.key);
  EXPECT_EQ("vec", object_copy.vec);
  EXPECT_EQ("Algorithm name for encoding", object_copy.algorithm_name);
}
