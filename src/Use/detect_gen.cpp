#include "Simd/SimdDetection.hpp"
#include "Simd/SimdDrawing.hpp"
#include <iostream>
int main(int argc, char** argv)
{
    typedef Simd::Detection<Simd::Allocator> Detection;
 
    Detection::View image,view;
    std::ofstream gold_file;
    Detection detection;
    int temp;
    detection.Load(argv[1]);//"../../data/cascade/haar_face_0.xml"
    for(int fig_num = 4; fig_num < argc;fig_num++){
        image.Load(argv[fig_num]);
        detection.Init(image.Size(),1.1,Detection::Size(0, 0),Detection::Size(INT_MAX, INT_MAX),view,atoi(argv[2]));
        Detection::Objects objects;
        detection.Detect(image, objects); 
        gold_file.open(argv[3],std::ios::out | std::ios::binary| std::ios::trunc);
        temp= objects.size();
        gold_file.write((char*)&temp,sizeof(int));
        for (size_t i = 0; i < objects.size(); i++){
            //Simd::DrawRectangle(image, (int)objects[i].rect, uint8_t(255));
            temp=(int)(int)objects[i].rect.Left();
            gold_file.write((char*)&temp,sizeof(int));
            temp=(int)objects[i].rect.Top();
            gold_file.write((char*)&temp,sizeof(int));
            temp=(int)objects[i].rect.Right();
            gold_file.write((char*)&temp,sizeof(int));
            temp=(int)objects[i].rect.Bottom();
            gold_file.write((char*)&temp,sizeof(int));
            temp=(int)objects[i].weight;
            gold_file.write((char*)&temp,sizeof(int));
            std::cout << (int)objects[i].rect.Left() << " "<< (int)objects[i].rect.Top() << " "  
            << (int)objects[i].rect.Right()<<" "<< (int)objects[i].rect.Bottom() << std::endl;        
        }   
        
    }
    gold_file.close();
    //image.Save("result.pgm");
    return 0;
}