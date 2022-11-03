#pragma once

#include<unordered_map>
#include<iostream>
#include<stdio.h>

//框架读取配置文件类
class MprpcConfig 
{
private:
    std::unordered_map<std::string,std::string>unmap;
public:
    void LoadConfigFile(const char*config_file);
    std::string Load(const std::string key);
    MprpcConfig(/* args */);
    ~MprpcConfig();
};



