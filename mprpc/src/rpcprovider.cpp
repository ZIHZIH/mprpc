#include "rpcprovider.h"


void RpcProvider::NotifyService(google::protobuf::Service *service) {
    ServiceInfo service_info;
    
    //GET描述信息
    const google::protobuf::ServiceDescriptor*pserviceDesc=service->GetDescriptor();

    //获得服务名字
    const std::string service_name=pserviceDesc->name();

    //获取方法个数
    int methodcount=pserviceDesc->method_count();
    for(int i=0;i<methodcount;i++){
        const google::protobuf::MethodDescriptor*pmethodDesc=pserviceDesc->method(i); 
        std::string method_name=pmethodDesc->name();
        std::cout<<method_name<<std::endl;
        service_info.m_methodmap.insert({method_name,pmethodDesc});
    }
    service_info.m_service=service;
    
    m_servicemap.insert({service_name,service_info});


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

    //注册至服务注册中心
    ZkClient zk_client;
    zk_client.Start();
    for(auto&service_it:m_servicemap ){
        std::string path="/"+service_it.first;
        zk_client.Create(path.c_str(),nullptr,0);

        for(auto&method_it:service_it.second.m_methodmap){
            path+=("/"+method_it.first);
            char method_path[128]={0};
            sprintf(method_path,"%s:%d",ip.c_str(),port);
            zk_client.Create(path.c_str(),method_path,strlen(method_path),ZOO_EPHEMERAL);
        }
    }    


    //启动网络服务
    server.start();
    m_eventloop.loop();


}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn){
    if(!conn->connected()){
        conn->shutdown();
    }
}

void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn,muduo::net::Buffer*buffer,muduo::Timestamp time){
    //接收的字符流 处理字符流
    std::string recv_buf=buffer->retrieveAllAsString();

    uint32_t header_size=0;
    recv_buf.copy((char *)&header_size,4,0);

    std::string rpc_header_str=recv_buf.substr(4,header_size);

    mprpc::RpcHeader rpcHeader;
    std::string service_name;
    std::string method_name;
    uint32_t args_size;

    if(rpcHeader.ParseFromString(rpc_header_str)){
        service_name=rpcHeader.service_name();
        method_name=rpcHeader.method_name();
        args_size=rpcHeader.args_size();

    }else{
        std::cout<<"rpcHeader ParseFromString error"<<std::endl;
        return;
    }

    std::string args_str=recv_buf.substr(4+header_size,args_size);

    //获取service对象和method方法
    auto it=m_servicemap.find(service_name);
    if(it==m_servicemap.end()){
        std::cout<<"service is not exist"<<std::endl;
        return;
    }

    
    auto mit=it->second.m_methodmap.find(method_name);
    if(mit==it->second.m_methodmap.end()){
        std::cout<<"method is not exist"<<std::endl;
        return;
    }

    google::protobuf::Service *service=it->second.m_service;
    const google::protobuf::MethodDescriptor*method=mit->second;

    //生成rpc方法调用相应的request参数和reponse参数
    google::protobuf::Message *request=service->GetRequestPrototype(method).New();
    google::protobuf::Message*response=service->GetResponsePrototype(method).New();

    //装弹(填参)
    if(!(request->ParseFromString(args_str))){
        std::cout<<"request ParseFromString error"<<std::endl;
        return;
    }

    google::protobuf::Closure*done=google::protobuf::NewCallback<RpcProvider,const muduo::net::TcpConnectionPtr &,google::protobuf::Message*>(this,&RpcProvider::SendRpcResponse,conn,response);

    //发射
    service->CallMethod(method,nullptr,request,response,done);

}

//Closure的回调函数 done->Run();
void RpcProvider::SendRpcResponse( const muduo::net::TcpConnectionPtr&conn,google::protobuf::Message*response){
    std::string send_str;
    if(response->SerializeToString(&send_str)){
        conn->send(send_str);
    }else{
        std::cout<<"SendRpcRespoonse response SerializeAsString error"<<std::endl;
    }
    conn->shutdown();
}
 