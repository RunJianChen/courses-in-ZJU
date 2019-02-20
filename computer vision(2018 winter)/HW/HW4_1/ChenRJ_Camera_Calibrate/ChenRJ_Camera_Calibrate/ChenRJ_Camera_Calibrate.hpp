//
//  ChenRJ_Camera_Calibrate.hpp
//  Camera_Calibrating
//
//  Created by Chen~_~RJ on 2018/12/25.
//  Copyright © 2018 Chen~_~RJ. All rights reserved.
//

#ifndef ChenRJ_Camera_Calibrate_hpp
#define ChenRJ_Camera_Calibrate_hpp

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <vector>

#include "opencv/cxcore.h"
#include "opencv/highgui.h"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv/cv.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace std;
using namespace cv;

class ChenRJ_Camera_Calibrate
{
private:
    Mat Intrinsic_Parameter;
    Mat Distorn_Parameter;
    Mat Extrinsic_Parameter_r,Extrinsic_Parameter_t;
    
    vector<Mat> Raw_Data;
    
    void Calculate_Parameter();
    
    int Board_Width,Board_Height,Board_Size;
    
public:
    ChenRJ_Camera_Calibrate(vector<Mat>* Input_Data,int H,int W):Raw_Data(*Input_Data)
    {
        Raw_Data = *Input_Data;
        Board_Height = H;
        Board_Width = W;
        Board_Size = Board_Width * Board_Height;
        Calculate_Parameter();
    }
    ~ChenRJ_Camera_Calibrate(){};
    Mat Bird_View(Mat Source);
    Mat Get_Intrinsic_Parameter()
    {
        return Intrinsic_Parameter;
    }
    Mat Get_Distorn_Parameter()
    {
        return Distorn_Parameter;
    }
    void Get_Extrinsic_Parameter(Mat *R,Mat *t)
    {
        *R = Extrinsic_Parameter_r;
        *t = Extrinsic_Parameter_t;
    }
};


#endif /* ChenRJ_Camera_Calibrate_hpp */
