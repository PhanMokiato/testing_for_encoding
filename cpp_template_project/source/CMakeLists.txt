add_executable(my_main main.cpp)

set(HEADER_LIST "../include/encoding_library/encode_lib.h")

add_library(en_library encode_lib.cpp ${HEADER_LIST})

set_target_properties(en_library PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(en_library PUBLIC ../include/encoding_library)

target_link_libraries(my_main en_library)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})

