include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib)

add_executable(test_rig_scp test_main.c
                            test_queue.c
                            test_scp.c
                            test_rig_scp.c
                            mock_machine.c)
target_link_libraries(test_rig_scp check
                                   rigscp)
add_custom_target(run_tests valgrind -q --leak-check=full ./test_rig_scp
                            DEPENDS test_rig_scp)
