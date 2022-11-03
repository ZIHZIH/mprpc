#include <iostream>
#include <string>
#include "user.pb.h"
#include "mprpcapplication.h"

class UserService:public fixbug::UserServiceRpc
{     
public:
    bool Login(std::string name,std::string pwd){
        std::cout<<"doing Login func"<<std::endl;
        std::cout <<"name:"<<name<<"pwd:"<<pwd<<std::endl;

        return true;
    }

    void Login(::google::protobuf::RpcController* controller,
               const ::fixbug::LoginRequest* request,
               ::fixbug::LoginResponse* response,
               ::google::protobuf::Closure* done){

        std::string name=request->name();
        std::string pwd=request->pwd();

        //本地业务
        bool login_result=Login(name,pwd);

        //写响应
        fixbug::ResultCode *code=response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("default");

        response->set_success(login_result);

        //调用成功写个日志
        LOG_INFO("LOGIN SUCCESS");
        
        //执行回调， response的序列化和网络传输
        done->Run();

    }

};


int main(int argc,char **argv){

    MprpcApplication::Init(argc,argv);

    RpcProvider provider;
    provider.NotifyService(new UserService());
    provider.Run();

    std::cout<<"success"<<std::endl;

    return 0;
}

