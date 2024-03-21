add_executable(my_main main.cpp)

set(HEADER_LIST "../include/encoding_library/encode_lib.h")
set(HEADER_LIST_2 "../include/encoding_library/encode_constants.h")

add_library(en_library encode_lib.cpp ${HEADER_LIST})
add_library(en_library_const ${HEADER_LIST_2})

set_target_properties(en_library PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(en_library_const PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(en_library PUBLIC ../include/encoding_library)

target_include_directories(en_library_const PUBLIC ../include/encoding_library)

target_link_libraries(my_main 
    en_library
    en_library_const)

