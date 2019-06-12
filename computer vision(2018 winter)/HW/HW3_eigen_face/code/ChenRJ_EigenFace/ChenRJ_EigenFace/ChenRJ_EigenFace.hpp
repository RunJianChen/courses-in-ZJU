//
//  ChenRJ_EigenFace.hpp
//  ChenRJ_EigenFace
//
//  Created by Chen~_~RJ on 2018/12/10.
//  Copyright © 2018 Chen~_~RJ. All rights reserved.
//

#ifndef ChenRJ_EigenFace_hpp
#define ChenRJ_EigenFace_hpp

#include "opencv/cxcore.h"
#include "opencv/highgui.h"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv/cv.h"
#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;
using namespace Eigen;

#define Max_Featured_Face 100
#define H 112
#define W 92

class ChenRJ_EigenFace
{
private:
    int Subspace_Dimension,Common_H,Common_W;//dimension of subspace   common Height and Width for data
    
    Mat Transform_to_Subspace;//transform matrix A
    
    vector<Mat> Origin_Data;//original data
    
    vector<Mat> Database;//database
    
    Mat im_Avg;//average image
    
    void Pre_Process();//pre process
    
    void Train();//train
    
    void Calculate_Average(vector<float> *Average_Face);
    
    void Calculate_Trans_Matrix(int Subspace_Dimension,vector<float> *Average_Face);
    
    int Get_Index(int Height,int Width);
    
public:
    ChenRJ_EigenFace(vector<Mat> *Training_data,double Energy_Rate);
    ~ChenRJ_EigenFace(){};
    int Indentification(Mat New_face);//identification
    Mat Get_Face_from_Database(int Index);
    Mat Reconstruct(Mat Reconstruct_Source,double chosen_rate);//reconstruct
};

#endif /* ChenRJ_EigenFace_hpp */
