//
//  main.cpp
//  CV_Harris_Corner_Detector
//
//  Created by Chen~_~RJ on 2018/12/3.
//  Copyright © 2018 Chen~_~RJ. All rights reserved.
//

#include "ChenRJ_Harris_Detector.hpp"

int main(int argc, const char * argv[]) {
    Mat new_image = imread("/Users/chenrj/Desktop/1.png");
    ChenRJ_Detector* de = new ChenRJ_Detector(new_image);
    
    new_image = de->Feature_Generator();
    
    imwrite("/Users/chenrj/Desktop/2.png", new_image);
    
}
