#include "rpcprovider.h"
#include<string>
#include"mprpcapplication.h"
#include<functional>

void RpcProvider::NotifyService(google::protobuf::Service *service) {

}


void RpcProvider::Run() {

    std::string ip=MprpcApplication::GetConfig().Load("rpcserverip");
    uint16_t port=atoi(MprpcApplication::GetConfig().Load("rpcserverport").c_str());
    muduo::net::InetAddress address(ip,port);

    //创建TcpServer对象
    muduo::net::TcpServer server(&m_eventloop,address,"RpcProvide");

    //绑定回调方法
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection,this,std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));

    //设置线程数量
    server.setThreadNum(4);

    std::cout<<"server start"<<std::endl;
    std::cout<<"rpcserver ip="<<ip<<std::endl<<"rpcserver port="<<port<<std::endl;
    //启动网络服务
    server.start();
    m_eventloop.loop();




}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn){

}

void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& coon,muduo::net::Buffer*buffer,muduo::Timestamp time){

}