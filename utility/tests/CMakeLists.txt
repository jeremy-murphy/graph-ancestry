find_package(Boost 1.49 REQUIRED system test_exec_monitor timer)
include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

add_executable(test_transformers test_transformers.cpp)
target_link_libraries(test_transformers ${Boost_LIBRARIES})
add_test(test_transformers test_transformers)
