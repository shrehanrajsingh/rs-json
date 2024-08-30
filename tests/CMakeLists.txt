include_directories(../)
add_executable(TEST_EXE test.c)
target_link_libraries(TEST_EXE rsjson)
add_test(TEST TEST_EXE)