#include<iostream>
#include "test.pb.h"
#include <string>

using namespace fixbug;
int main(){
    fixbug::loginrequest req;
    req.set_name("zhang san");
    req.set_pwd("123456");

    std::string send_str;
    req.SerializeToString(&send_str);
    std::cout<<send_str.c_str()<<std::endl;

    loginrequest reqB;
    bool ret;
    if(ret=reqB.ParseFromString(send_str)){
        std::cout<<reqB.name()<<std::endl;
        std::cout<<reqB.pwd()<<std::endl;
    }else{
        std::cout<<"error"<<std::endl;
    }

    return 0;
}
