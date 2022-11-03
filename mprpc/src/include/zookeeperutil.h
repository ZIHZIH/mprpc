#pragma once

#include<semaphore.h>
#include<zookeeper/zookeeper.h>
#include<string>
#include<iostream>
#include"mprpcapplication.h"

class ZkClient{

public:
    ZkClient();
    ~ZkClient();

    //zkclient启动连接zkserver
    void Start();

    //在zkserver上根据path创建znode节点
    void Create(const char *path, const char *data, int datalen, int state=0);

    //根据节点path获取节点上的内容
    std::string GetData(const char *path);

private:
    //zk的客户端句柄
    zhandle_t *m_zhandle;
};
