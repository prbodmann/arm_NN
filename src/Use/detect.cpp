#include "Simd/SimdDetection.hpp"
#include "Simd/SimdDrawing.hpp"
#include <iostream>
#include<arpa/inet.h>
#include<sys/socket.h>


unsigned int buffer[100];
int s;
struct sockaddr_in server;

void setup_socket(char* ip_addr, int port){
    s=socket(PF_INET, SOCK_DGRAM, 0);
    //memset(&server, 0, sizeof(struct sockaddr_in));
    //printf("port: %d",port);
    //printf("ip: %s", ip_addr);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip_addr);

}

void send_message(size_t size){
    sendto(s,buffer,4*size,0,(struct sockaddr *)&server,sizeof(server));
}



int main(int argc, char** argv)
{
    typedef Simd::Detection<Simd::Allocator> Detection;
    setup_socket(argv[1],atoi(argv[2]));    
    Detection::View image;
    std::ifstream gold_file;
    std::vector<int> gold;
    Detection detection;
    int temp,flag;
    int gold_iter;
    detection.Load(argv[3]);//"../../data/cascade/haar_face_0.xml"
    gold_file.open(argv[5],std::ios::in | std::ios::binary);
    for(int j=0;j<3;j++){
        gold_file.read((char*)&temp,sizeof(int));
        gold.push_back(temp);
    }
    gold_file.close();
    while(1){
        gold_iter=0;
        for(int fig_num = 4; fig_num < argc;fig_num++){
            image.Load(argv[fig_num]);
            detection.Init(image.Size(),atoi(argv[4]));
            Detection::Objects objects;
            detection.Detect(image, objects); 
            flag==0;
            if(objects.size()==0){
                if(gold.at(gold_iter)!=-1||gold.at(gold_iter+1)!=-1||gold.at(gold_iter+2)!=-1||gold.at(gold_iter+3)!=-1
                    ||gold.at(gold_iter+4)!=-1||gold.at(gold_iter+5)!=-1||gold.at(gold_iter+6)!=-1||gold.at(gold_iter+7)!=-1){
                    if(flag==0){
                        buffer[0]=0xDD000000;
                    }else{
                        flag=1;
                        buffer[0]=0xCC000000;
                    }
                    flag=1;
                    buffer[1]=0xFFFFFFFF;
                    buffer[2]=0xFFFFFFFF;
                    buffer[3]=0xFFFFFFFF;
                    buffer[4]=0xFFFFFFFF;
                    buffer[5]=0xFFFFFFFF;
                    buffer[6]=0xFFFFFFFF;
                    buffer[7]=0xFFFFFFFF;
                    buffer[8]=0xFFFFFFFF;      
                    send_message(9);
                        
                }
                
            }else{
                for (size_t i = 0; i < objects.size(); i++, gold_iter+=8){
                    if(gold.at(gold_iter)!=(int)objects[i].rect.TopLeft().x||gold.at(gold_iter+1)!=(int)objects[i].rect.TopLeft().y
                        ||gold.at(gold_iter+2)!=(int)objects[i].rect.TopRight().x||gold.at(gold_iter+3)!=(int)objects[i].rect.TopRight().y
                        ||gold.at(gold_iter+4)!=(int)objects[i].rect.BottomLeft().x||gold.at(gold_iter+5)!=(int)objects[i].rect.BottomLeft().y||
                        gold.at(gold_iter+6)!=(int)objects[i].rect.BottomRight().x||gold.at(gold_iter+7)!=(int)objects[i].rect.BottomRight().y){
                        if(flag==0){
                            buffer[0]=0xDD000000;
                        }else{
                            flag=1;
                            buffer[0]=0xCC000000;
                        }
                        temp=(int)objects[i].rect.TopLeft().x;
                        buffer[1]=*((unsigned int*)&temp);
                        temp=(int)objects[i].rect.TopLeft().y;  
                        buffer[2]=*((unsigned int*)&temp);
                        temp=(int)objects[i].rect.TopRight().x;
                        buffer[3]=*((unsigned int*)&temp);
                        temp=(int)objects[i].rect.TopRight().y;
                        buffer[4]=*((unsigned int*)&temp);
                        temp=(int)objects[i].rect.BottomLeft().x;
                        buffer[5]=*((unsigned int*)&temp);
                        temp=(int)objects[i].rect.BottomLeft().y;
                        buffer[6]=*((unsigned int*)&temp);
                        temp=(int)objects[i].rect.BottomRight().x;
                        buffer[7]=*((unsigned int*)&temp);
                        temp=(int)objects[i].rect.BottomRight().y;
                        buffer[8]=*((unsigned int*)&temp);  
                        send_message(9);
                            
                    }
                }
            } 
            if(flag==0){
                buffer[0] = 0xAA000000; //sem erros
                send_message(1);
            }  
        
        }
    }
    
    
    //image.Save("result.pgm");
    return 0;
}