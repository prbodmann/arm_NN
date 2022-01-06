#include "Simd/SimdDetection.hpp"
#include "Simd/SimdDrawing.hpp"
#include <iostream>
int main(int argc, char** argv)
{
    typedef Simd::Detection<Simd::Allocator> Detection;
 
    Detection::View image;

    image.Load(argv[2]);//"../../data/image/face/pp.pgm"
 
    Detection detection;
 
    detection.Load(argv[1]);//"../../data/cascade/haar_face_0.xml"
    detection.Init(image.Size(),atoi(argv[3]));
    Detection::Objects objects;
    detection.Detect(image, objects);    
    for (size_t i = 0; i < objects.size(); ++i){
        Simd::DrawRectangle(image, objects[i].rect, uint8_t(255));
    }
    image.Save("result.pgm");
 
    return 0;
}