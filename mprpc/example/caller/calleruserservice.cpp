#include<iostream>
#include"user.pb.h"
#include"mprpcapplication.h"
#include"mprpcchannel.h"
#include"mprpccontroller.h"

int main(int argc ,char**argv){

    //框架初始化
    MprpcApplication::Init(argc,argv);

    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());

    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");

    MprpcController controller;

    fixbug::LoginResponse response;
    stub.Login(&controller,&request,&response,nullptr);

    if(controller.Failed()){
        std::string error_text=controller.ErrorText();
        std::cout<<error_text<<std::endl;
    }else{
        if(response.result().errcode()==0){
        std::cout<<"success"<<std::endl;
        std::cout<<"return result:"<<response.success()<<std::endl;
        }else{
        std::cout<<"error"<<std::endl;
        std::cout<<response.result().errmsg()<<std::endl;
        }
    }

   

    return 0;
}