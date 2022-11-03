set(SRC_LIST ../user.pb.cc calleruserservice.cpp)
add_executable(consumer ${SRC_LIST})
target_link_libraries(consumer mprpc protobuf)