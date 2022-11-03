#pragma once
#include"mprpcconfig.h"
#include"rpcprovider.h"
#include <iostream>
#include<unistd.h>
#include<string>
#include"logger.h"

class MprpcApplication{
public:
    static void Init(int argc,char **argv);
    static MprpcApplication& GetInstance();
    static MprpcConfig& GetConfig();
private:
    static MprpcConfig m_config;
    MprpcApplication(){};
    MprpcApplication(const MprpcApplication&)=delete;
    MprpcApplication(MprpcApplication&&)=delete;
};

