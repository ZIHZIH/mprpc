#include"mprpcchannel.h"

void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                google::protobuf::RpcController* controller, 
                const google::protobuf::Message* request,
                google::protobuf::Message* response, google::protobuf::Closure* done)
{
    //序列化
    const google::protobuf::ServiceDescriptor* sd=method->service();

    std::string service_name=sd->name();
    std::string method_name=method->name();

    int args_size=0;
    std::string args_str;

    if(request->SerializeToString(&args_str)){
        args_size=args_str.size();
    }else{
        controller->SetFailed("request SerializeToString error");
        return;
    }

    mprpc::RpcHeader rpc_header;
    rpc_header.set_service_name(service_name);
    rpc_header.set_method_name(method_name);
    rpc_header.set_args_size(args_size);

    uint32_t header_size=0;
    std::string rpc_header_str;
    if(rpc_header.SerializeToString(&rpc_header_str)){
        header_size=rpc_header_str.size();
    }else{
        controller->SetFailed("rpc_header SerializeToString error");
        return;
    }

    std::string send_str;

    send_str.insert(0,std::string((char*)(&header_size),4));
    send_str+=rpc_header_str;
    send_str+=args_str;

    //连接服务注册中心进行查询
    ZkClient zkCli;
    zkCli.Start();
    std::string method_path = "/" + service_name + "/" + method_name;
    std::string host_data = zkCli.GetData(method_path.c_str());
    
    if(host_data==""){
        controller->SetFailed(method_path + " is not exist!");
        return;
    }

    int idx = host_data.find(":");
    if (idx == -1){
        controller->SetFailed(method_path + " address is invalid!");
        return;
    }

    std::string ip = host_data.substr(0, idx);
    uint16_t port = atoi(host_data.substr(idx+1, host_data.size()-idx).c_str());

    //tcp传输

    int clientfd=socket(AF_INET,SOCK_STREAM,0);
    if(clientfd==-1){
        char buf[1024]={0};
        sprintf(buf,"creat socket error:%d",errno);
        controller->SetFailed(buf);
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(port);
    server_addr.sin_addr.s_addr=inet_addr(ip.c_str());

    if(connect(clientfd,(sockaddr *)&server_addr,sizeof(server_addr))==-1){
        char buf[1024]={0};
        sprintf(buf,"connect error:%d",errno);
        controller->SetFailed(buf);
        close(clientfd);
        return;
    }

    if(send(clientfd,send_str.c_str(),send_str.size(),0)==-1){
        controller->SetFailed("send error");
        close(clientfd);
        return;
    }

    char buf[1024]={0};
    int rece_size=0;
    if((rece_size=recv(clientfd,buf,sizeof(buf),0))==-1){
        controller->SetFailed("recv error");
        close(clientfd);
        return;
    }

    
    if(!(response->ParseFromArray(buf,rece_size))){
        controller->SetFailed("response ParseFromString error");
        close(clientfd);
        return;
    }

    close(clientfd);
}