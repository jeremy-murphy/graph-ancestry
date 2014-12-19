find_package(Boost 1.49 REQUIRED graph system test_exec_monitor timer)
include_directories(".." "../utility/" ${Boost_INCLUDE_DIRS})

add_executable(test_lca test_lca.cpp)
target_link_libraries(test_lca ${Boost_LIBRARIES})
add_test(test_lca test_lca)

add_executable(test_RMQ test_RMQ.cpp)
target_link_libraries(test_RMQ ${Boost_LIBRARIES})
add_test(test_RMQ test_RMQ)

add_executable(test_graph_visitors test_graph_visitors.cpp)
target_link_libraries(test_graph_visitors ${Boost_LIBRARIES})
add_test(test_graph_visitors test_graph_visitors)
