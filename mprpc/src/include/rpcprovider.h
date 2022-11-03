#pragma once

#include <google/protobuf/service.h>
#include<muduo/net/TcpServer.h>
#include<muduo/net/EventLoop.h>
#include<muduo/net/InetAddress.h>
#include<muduo/net/TcpConnection.h> 
#include<string>
#include"mprpcapplication.h"
#include<functional>
#include<google/protobuf/descriptor.h>
#include<unordered_map>
#include"rpcheader.pb.h"
#include"zookeeperutil.h"

class RpcProvider{
public:
    void Run();
    void NotifyService(google::protobuf::Service *service);

private:
    muduo::net::EventLoop m_eventloop;

    //新的socket连接回调
    void OnConnection(const muduo::net::TcpConnectionPtr&);
    //读写事件回调
    void OnMessage(const muduo::net::TcpConnectionPtr&,muduo::net::Buffer*,muduo::Timestamp);

    //服务类型信息
    struct ServiceInfo{
        google::protobuf::Service *m_service;
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*>m_methodmap;
    };

    std::unordered_map<std::string,ServiceInfo>m_servicemap;

    void SendRpcResponse(const muduo::net::TcpConnectionPtr &conn,google::protobuf::Message*response);

};
