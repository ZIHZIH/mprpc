#include"mprpcapplication.h"
#include <iostream>
#include<unistd.h>
#include<string>

MprpcConfig MprpcApplication::m_config;



void  ConfigErrorMsg(){
    std::cout<<"Config Error"<<std::endl;
    std::cout<<"format:command -i <configfile>"<<std::endl;
}

void  MprpcApplication::Init(int argc,char **argv){
    //判断配置文件
    if(argc<2){
        ConfigErrorMsg();
        exit(-1);
    }

    std::string config_file;
    int c=0;

    while((c=getopt(argc,argv,"i:"))!=-1){
        switch (c)
        {
        case '?':
            ConfigErrorMsg();
            exit(-1);
        case 'i':
            config_file=optarg;
            break;
        default:
            break;
        }
    }

    m_config.LoadConfigFile(config_file.c_str());
    std::cout<<"rpcserverip="<<m_config.Load("rpcserverip")<<std::endl;
    std::cout<<"rpcserverport="<<m_config.Load("rpcserverport")<<std::endl;
    std::cout<<"zookeeperip="<<m_config.Load("zookeeperip")<<std::endl;
    std::cout<<"zookeeperport="<<m_config.Load("zookeeperport")<<std::endl;

 }


MprpcApplication& MprpcApplication::GetInstance(){
    static MprpcApplication app;
    return app;
}

MprpcConfig& MprpcApplication::GetConfig(){
    return m_config;
}


