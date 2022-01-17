#include "Simd/SimdDetection.hpp"
#include "Simd/SimdDrawing.hpp"
#include <iostream>
int main(int argc, char** argv)
{
    typedef Simd::Detection<Simd::Allocator> Detection;
 
    Detection::View image;
    std::ofstream gold_file;

    Detection detection;
    int temp;
    detection.Load(argv[1]);//"../../data/cascade/haar_face_0.xml"
    for(int fig_num = 3; fig_num < argc;fig_num++){
        image.Load(argv[fig_num]);
        detection.Init(image.Size(),atoi(argv[2]));
        Detection::Objects objects;
        detection.Detect(image, objects); 
        gold_file.open("detect_golden.bin",std::ios::out | std::ios::binary| std::ios::trunc);
        if(objects.size()==0){
            temp=-1;
            for(int j=0;j<3;j++){
                gold_file.write((char*)&temp,sizeof(int));
            }
            
        }else{
            for (size_t i = 0; i < objects.size(); i++){
                //Simd::DrawRectangle(image, (int)objects[i].rect, uint8_t(255));
                temp=(int)(int)objects[i].rect.TopLeft().x;
                gold_file.write((char*)&temp,sizeof(int));
                temp=(int)objects[i].rect.TopLeft().y;
                gold_file.write((char*)&temp,sizeof(int));
                temp=(int)objects[i].rect.TopRight().x;
                gold_file.write((char*)&temp,sizeof(int));
                temp=(int)objects[i].rect.TopRight().y;
                gold_file.write((char*)&temp,sizeof(int));
                temp=(int)objects[i].rect.BottomLeft().x;
                gold_file.write((char*)&temp,sizeof(int));
                temp=(int)objects[i].rect.BottomLeft().y;
                gold_file.write((char*)&temp,sizeof(int));
                temp=(int)objects[i].rect.BottomRight().x;
                gold_file.write((char*)&temp,sizeof(int));
                temp=(int)objects[i].rect.BottomRight().y;
                gold_file.write((char*)&temp,sizeof(int));
                std::cout << (int)objects[i].rect.TopLeft().x << " "<< (int)objects[i].rect.TopLeft().y << " "  
                << (int)objects[i].rect.TopRight().x << " "<< (int)objects[i].rect.TopRight().y << " "
                << (int)objects[i].rect.BottomLeft().x  << " " << (int)objects[i].rect.BottomLeft().y  << " " 
                << (int)objects[i].rect.BottomRight().x <<" "<< (int)objects[i].rect.BottomRight().y << std::endl;
            }
        }   
        
    }
    gold_file.close();
    //image.Save("result.pgm");
    return 0;
}