//
//  ChenRJ_Harris_Detector.hpp
//  CV_Harris_Corner_Detector
//
//  Created by Chen~_~RJ on 2018/12/3.
//  Copyright © 2018 Chen~_~RJ. All rights reserved.
//

#ifndef ChenRJ_Harris_Detector_hpp
#define ChenRJ_Harris_Detector_hpp

#include "opencv/cxcore.h"
#include "opencv/highgui.h"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv/cv.h"
#include <math.h>
#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace Eigen;


class ChenRJ_Detector
{
private:
    Mat prey;
    int patch_H,patch_W;
    double *patch_factor,*R_value,*X_gradient,*Y_gradient,R_Threshold,*Max_Eigen_Value,*Min_Eigen_Value;
    void Gradient_Calculator();
    void R_Calculator(double empirical_const);
    bool Local_Maximum(int x,int y);
    int Get_Index(int x,int y);
    
public:
    ChenRJ_Detector(Mat image,int H = 9 ,int W = 9):prey(image),patch_H(H),patch_W(W)
    {
        patch_factor = new double[patch_H*patch_W];
        for (int i = 0 ; i < H ; i++)
            for (int j = 0 ; j < W ; j++)
            if ((i!=H/2) || (j!=W/2))
            {
                patch_factor[i*W+j] = 1.0 / ( (i-H/2)*(i-H/2) + (j-W/2)*(j-W/2) );
            }
            else
                patch_factor[i*W+j] = 2.0;
        R_value = new double[image.cols*image.rows];
        X_gradient = new double[image.cols*image.rows];
        Y_gradient = new double[image.cols*image.rows];
        Max_Eigen_Value = new double[image.cols*image.rows];
        Min_Eigen_Value = new double[image.cols*image.rows];
        R_Threshold = 0;
    }
    ~ChenRJ_Detector();
    Mat Feature_Generator();
};

#endif /* ChenRJ_Harris_Detector_hpp */
