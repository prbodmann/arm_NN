#include "Simd/SimdDetection.hpp"
#include "Simd/SimdDrawing.hpp"
#include <iostream>
#include<arpa/inet.h>
#include<sys/socket.h>
using namespace std;

#define ERR_DETECT 0
#define MISS_DETECT 1
typedef Simd::Detection<Simd::Allocator> Detection;

unsigned int buffer[100];
int s_int;
struct sockaddr_in server;

void setup_socket(char* ip_addr, int port){
    s_int=socket(PF_INET, SOCK_DGRAM, 0);
    //memset(&server, 0, sizeof(struct sockaddr_in));
    //printf("port: %d",port);
    //printf("ip: %s", ip_addr);
    server.sin_family = AF_INET;
    server.sin_port = htons(port); 
    server.sin_addr.s_addr = inet_addr(ip_addr);

}

void send_message(size_t size){
    for(int i=0; i< size;i++){    cout << hex << buffer[i] <<" ";   }
    cout << endl;
                 //sendto(s_int,buffer,4*size,0,(struct sockaddr *)&server,sizeof(server));
}



 Detection::Objects gold;
int main(int argc, char** argv)
{
    
    setup_socket(argv[1],atoi(argv[2]));    

    ifstream gold_file;
    Detection::View image,dummy_view;
    Detection detection;
    Detection::Objects objects;
    int temp[6],flag, fig_num, gold_iter,i;
    vector<int> gold_size;
    vector<Detection::Object>::iterator j;

    detection.Load(argv[3]);//"../../data/cascade/haar_face_0.xml"
    gold_file.open(argv[5],ios::in | ios::binary);

    while(1){
        if(!gold_file.read((char*)temp,sizeof(int))){
            break;
        }
        gold_size.push_back(temp[0]);
        for(int i=0;i<gold_size.back();i++){
            gold_file.read((char*)temp,5*sizeof(int));
            Simd::Rectangle <int>rect= Simd::Rectangle<int>(temp[0],temp[1],temp[2],temp[3]);
            Detection::Object obj = Detection::Object(rect,temp[4]);
            gold.push_back(obj);
        }       
    }
    gold_file.close();
    //cout << gold_size[0]<< " "<< gold_size.size()<<" "<< gold.size()<<endl;

    while(1){
        flag=0;
        j=gold.begin();
        for(fig_num= 0; fig_num < argc-6;++fig_num){

            cout << argv[fig_num+6] << endl;
            image.Load(argv[fig_num+6]);
            detection.Init(image.Size(),1.1,Detection::Size(0, 0),Detection::Size(INT_MAX, INT_MAX),dummy_view,atoi(argv[4]));
            detection.Detect(image, objects);        
            //cout << objects.size()<<endl;
            for(i=0;i<gold_size[fig_num];i++,++j){
                if(objects.size()==0){//pega detecçoes que faltaram

                    if(flag==0){                        
                        buffer[0]=0xDD000000;
                    }else{
                        buffer[0]=0xCC000000;
                    } 

                    buffer[1]=fig_num;
                    buffer[2]=MISS_DETECT;
                    buffer[3]=*((unsigned int*)&flag);
                    temp[0]=(*j).rect.Left();
                    buffer[4]=*((unsigned int*)&temp);
                    temp[0]=(*j).rect.Top();
                    buffer[5]=*((unsigned int*)&temp);
                    temp[0]=(*j).rect.Right();
                    buffer[6]=*((unsigned int*)&temp);
                    temp[0]= (*j).rect.Bottom();
                    buffer[7]=*((unsigned int*)&temp);
                    temp[0]=(*j).weight;
                    buffer[8]=*((unsigned int*)&temp);
                    send_message(9); 
                    ++flag;         
                }else{            
                    for(vector<Detection::Object>::iterator obj_i=objects.begin();obj_i<objects.end();++obj_i){                    
                        if((*j).rect.Left()==(*obj_i).rect.Left() && 
                            (*j).rect.Top()==(*obj_i).rect.Top() &&
                            (*j).rect.Right()==(*obj_i).rect.Right() &&
                            (*j).rect.Bottom()==(*obj_i).rect.Bottom() &&
                            (*j).weight==(*obj_i).weight){
                            objects.erase(obj_i);
                            break;                                                                     
                        }
                    }
                }                    
            }
            for(vector<Detection::Object>::iterator obj_i=objects.begin();obj_i<objects.end();++obj_i){      //detecção errada, pega tambem se detectou mais do que deveria e menos 
                //cout << distance(objects.begin(),obj_i) << endl;

                if(flag==0){
                    buffer[0]=0xDD000000;
                }else{
                    buffer[0]=0xCC000000;
                }
          
                buffer[1]=fig_num;
                buffer[2]=ERR_DETECT;
                buffer[3]=*((unsigned int*)&flag);
                temp[0]=(*obj_i).rect.Left();
                buffer[4]=*((unsigned int*)&temp);
                temp[0]=(*obj_i).rect.Top();
                buffer[5]=*((unsigned int*)&temp);
                temp[0]=(*obj_i).rect.Right();
                buffer[6]=*((unsigned int*)&temp); 
                temp[0]=(*obj_i).rect.Bottom();
                buffer[7]=*((unsigned int*)&temp);
                temp[0]=(*obj_i).weight;
                buffer[8]=*((unsigned int*)&temp);    
                send_message(9); 
                ++flag;
            }   
            /*if(flag!=0){
                buffer[0]=0xCC000000;
                buffer[1]=*((unsigned int*)flag);
                buffer[2]=*((unsigned int*)flag);
                buffer[3]=*((unsigned int*)flag);
                buffer[4]=*((unsigned int*)flag);
                buffer[5]=*((unsigned int*)flag);
                buffer[6]=*((unsigned int*)flag);
                buffer[7]=*((unsigned int*)flag);
                send_message(8);   
            }*/
        
           
        }
        if(flag==0){
            buffer[0] = 0xAA000000; //sem erros
            send_message(1);
        }
  
    }
    
    
    //image.Save("result.pgm");
    return 0;
}