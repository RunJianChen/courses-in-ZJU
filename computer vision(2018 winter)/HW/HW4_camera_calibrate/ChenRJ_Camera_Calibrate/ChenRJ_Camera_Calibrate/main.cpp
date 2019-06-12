//
//  main.cpp
//  ChenRJ_Camera_Calibrate
//
//  Created by Chen~_~RJ on 2018/12/26.
//  Copyright © 2018 Chen~_~RJ. All rights reserved.
//

#include "ChenRJ_Camera_Calibrate.hpp"

int main() {
    string filename = "/Users/chenrj/Downloads/手机拍的数据/";//LearningOpenCV_Code/birdseye/";
    vector<Mat> *input_data = new vector<Mat>(1);
    for (int i = 0 ; i < 1 ; i++)
    {
        string now = filename;
        now += to_string(5)+".jpg";
        (*input_data)[i] = imread(now);
    }
    
    ChenRJ_Camera_Calibrate new_calibrater = ChenRJ_Camera_Calibrate(input_data, 7, 6);
    
    
    //test
    imwrite("/Users/chenrj/Desktop/1.jpg", (*input_data)[0]);
    
    Mat bird = new_calibrater.Bird_View((*input_data)[0]);
    
    imwrite("/Users/chenrj/Desktop/3.jpg", bird);
    
    cout << "Intrinsic_Parameter : " << endl << new_calibrater.Get_Intrinsic_Parameter() << endl;
    
    cout << "Distorn_Parameter" << endl << new_calibrater.Get_Distorn_Parameter() << endl;
    
    Mat *R = new Mat,*t = new Mat;
    
    new_calibrater.Get_Extrinsic_Parameter(R, t);
    
    cout << "Extrinsic_Parameter_R : " << endl << *R << endl << "Extrinsic_Parameter_t : " << endl << *t << endl;
    
}
