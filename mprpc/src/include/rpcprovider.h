#pragma once

#include "google/protobuf/service.h"
#include<muduo/net/TcpServer.h>
#include<muduo/net/EventLoop.h>
#include<muduo/net/InetAddress.h>
#include<muduo/net/TcpConnection.h> 


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
};
