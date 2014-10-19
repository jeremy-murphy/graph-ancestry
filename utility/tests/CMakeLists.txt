find_package(Boost 1.49 REQUIRED system test_exec_monitor timer)
include_directories(".." ${Boost_INCLUDE_DIRS})

add_executable(test_algorithms test_algorithms.cpp)
target_link_libraries(test_algorithms ${Boost_LIBRARIES})
add_test(test_algorithms test_algorithms)

add_executable(test_transformers test_transformers.cpp)
target_link_libraries(test_transformers ${Boost_LIBRARIES})
add_test(test_transformers test_transformers)
