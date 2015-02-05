find_package(Boost 1.49 REQUIRED graph system test_exec_monitor timer)
include_directories(".." "../utility/" ${Boost_INCLUDE_DIRS})

add_executable(test_LCA lowest_common_ancestor.cpp)
target_link_libraries(test_LCA ${Boost_LIBRARIES})
add_test(test_LCA test_LCA)

add_executable(test_RMQ range_minimum_query.cpp)
target_link_libraries(test_RMQ ${Boost_LIBRARIES})
add_test(test_RMQ test_RMQ)

add_executable(test_graph_visitors visitors.cpp)
target_link_libraries(test_graph_visitors ${Boost_LIBRARIES})
add_test(test_graph_visitors test_graph_visitors)

add_executable(test_DAG find_common_ancestor.cpp)
target_link_libraries(test_DAG ${Boost_LIBRARIES})
add_test(test_DAG test_DAG)

add_executable(test_reflect reflect.cpp)
target_link_libraries(test_reflect ${Boost_LIBRARIES})
add_test(test_reflect test_reflect)

add_executable(test_common_ancestor_existence common_ancestor_existence.cpp)
target_link_libraries(test_common_ancestor_existence ${Boost_LIBRARIES})
add_test(test_common_ancestor_existence test_common_ancestor_existence)
