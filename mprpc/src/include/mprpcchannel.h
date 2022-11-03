#pragma once

#include<google/protobuf/service.h>
#include<google/protobuf/descriptor.h>
#include<google/protobuf/message.h>
#include<google/protobuf/stubs/callback.h>
#include"rpcheader.pb.h"
#include<string>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<errno.h>
#include"mprpcapplication.h"
#include<unistd.h>
#include"zookeeperutil.h"

class MprpcChannel:public google::protobuf::RpcChannel{

public:
   
void CallMethod(const google::protobuf::MethodDescriptor* method,
                google::protobuf::RpcController* controller, 
                const google::protobuf::Message* request,
                google::protobuf::Message* response, google::protobuf::Closure* done);



};