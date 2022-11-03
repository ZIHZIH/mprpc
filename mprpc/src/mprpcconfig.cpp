#include"mprpcconfig.h"


MprpcConfig::MprpcConfig(/* args */){}
MprpcConfig::~MprpcConfig(){}


    void  MprpcConfig:: LoadConfigFile(const char*config_file){
        FILE *pf=fopen(config_file,"r"); 

        if(pf==NULL){
            std::cout<<"fopen error"<<std::endl;
            exit(-1);
        }

        while(!feof(pf)){
            char buf[1024];
            fgets(buf,sizeof(buf),pf);

            std::string temp(buf);

            if(temp.empty() || temp[0]=='#'){
                continue;
            }

            int index=0;
            index=temp.find('=');
            if(index==-1){
                continue;
            }

            std::string key;
            std::string value;
            key=temp.substr(0,index);
            value=temp.substr(index+1,temp.size()-(index+2));
            if(temp.find('\n')==-1)value=temp.substr(index+1,temp.size()-(index+1));
            unmap.insert({key,value}); 
        }
    }

    std::string MprpcConfig::Load(const std::string key){
        auto it=unmap.find(key);
        if(it==unmap.end()){
            return "";
        }

        return unmap[key];
    }