//
//  ChenRJ_Harris_Detector.cpp
//  CV_Harris_Corner_Detector
//
//  Created by Chen~_~RJ on 2018/12/3.
//  Copyright © 2018 Chen~_~RJ. All rights reserved.
//

#include "ChenRJ_Harris_Detector.hpp"

Mat ChenRJ_Detector::Feature_Generator()
{
    Mat Feature_map = Mat::zeros(Size(prey.cols, prey.rows), CV_8UC3);

    Mat temp = Mat(prey);
    
    cvtColor(temp, prey, CV_BGR2GRAY);
    
    double empirical_const = 0.05;
    
    R_Calculator(empirical_const);
    
    for (int i = 0 ; i < prey.rows ; i++)
        for (int j = 0 ; j < prey.cols ; j++)
        {
            if (Local_Maximum(i,j))
            {
                unsigned char * pos = temp.ptr<unsigned char>(i,j);

                pos[0] = 0; pos[1] = 0; pos[2] = 255;
                pos[3] = 0; pos[4] = 0; pos[5] = 255;
                
                pos = temp.ptr<unsigned char>(i+1,j);
                
                pos[0] = 0; pos[1] = 0; pos[2] = 255;
                pos[3] = 0; pos[4] = 0; pos[5] = 255;
                
                pos = temp.ptr<unsigned char>(i-1,j);
                
                pos[0] = 0; pos[1] = 0; pos[2] = 255;
                pos[3] = 0; pos[4] = 0; pos[5] = 255;
                
            }
        }
    
    return temp;
}


void ChenRJ_Detector::Gradient_Calculator()
{
    int X_gra_factor[3][3]={-1,0,1,-2,0,2,-1,0,1},Y_gra_factor[3][3]={1,2,1,0,0,0,-1,-2,-1};
    
    Mat blur,X = (Mat_<double>( prey.rows,prey.cols)),Y  = (Mat_<double>( prey.rows,prey.cols));
    
    for (int i = patch_H/2 ; i < prey.rows-patch_H/2 ; i++)
        for (int j = patch_W/2 ; j < prey.cols-patch_W/2 ; j++)
        {
            double sum_x = 0,sum_y = 0;
            for (int k = -1 ; k <= 1 ; k++)
                for (int m = -1 ; m <= 1 ; m++)
                {
                    unsigned char * pos = prey.ptr<unsigned char>(i+k);
                    
                    sum_x += X_gra_factor[k+1][m+1] * pos[j+m];
                    
                    sum_y += Y_gra_factor[k+1][m+1] * pos[j+m];
                }
            
            X_gradient[i*prey.cols+j] = sum_x;
            
            Y_gradient[i*prey.cols+j] = sum_y;
        }
}

void ChenRJ_Detector::R_Calculator(double empirical_const)
{
    Gradient_Calculator();
    double max = 0,max_max = 0,max_min = 0;
    
    for (int i = patch_H/2 ; i < prey.rows-patch_H/2 ; i++)
        for (int j = patch_W/2 ; j < prey.cols-patch_W/2 ; j++)
        {
            double sum1_1 = 0,sum1_2 = 0,sum2_1 = 0,sum2_2 = 0;
            for (int k = -patch_H/2 ; k <= patch_H/2 ; k++)
                for (int m = -patch_W/2 ; m <= patch_W/2 ; m++)
                {
                    sum1_1 = sum1_1 + patch_factor[(k+patch_H/2)*patch_W+m+patch_W/2] * X_gradient[(i+k)*prey.cols+j+m]*X_gradient[(i+k)*prey.cols+j+m];
                    
                    sum1_2 = sum1_2 + patch_factor[(k+patch_H/2)*patch_W+m+patch_W/2] * X_gradient[(i+k)*prey.cols+j+m]*Y_gradient[(i+k)*prey.cols+j+m];
                    
                    sum2_1 = sum2_1 + patch_factor[(k+patch_H/2)*patch_W+m+patch_W/2] * X_gradient[(i+k)*prey.cols+j+m]*Y_gradient[(i+k)*prey.cols+j+m];
                    
                    sum2_2 = sum2_2 + patch_factor[(k+patch_H/2)*patch_W+m+patch_W/2] * Y_gradient[(i+k)*prey.cols+j+m]*Y_gradient[(i+k)*prey.cols+j+m];
                }
            R_value[i*prey.cols+j] = sum1_1*sum2_2 - sum2_1*sum1_2 - empirical_const * (sum1_1+sum2_2) * (sum1_1+sum2_2);
            
            Matrix2d A;
            A << sum1_1 , sum1_2 , sum2_1 , sum2_2;
            
            EigenSolver<Matrix<double, 2, 2>> es(A);
            MatrixXcd evecs = es.eigenvectors();
            MatrixXcd evals = es.eigenvalues();
            MatrixXd D(2,2);
            D=evals.real();
           // std::cout << D << std::endl;
            if (D(0) < D(1))
            {
                Max_Eigen_Value[i*prey.cols+j] = D(1);
                Min_Eigen_Value[i*prey.cols+j] = D(0);
            }
            else
            {
                Max_Eigen_Value[i*prey.cols+j] = D(0);
                Min_Eigen_Value[i*prey.cols+j] = D(1);
            }
            
            if (Max_Eigen_Value[i*prey.cols+j]>max_max) max_max = Max_Eigen_Value[i*prey.cols+j];
            if (Min_Eigen_Value[i*prey.cols+j]>max_min) max_min = Min_Eigen_Value[i*prey.cols+j];
           // std::cout << Max_Eigen_Value[i*prey.cols+j] << " " << Min_Eigen_Value[i*prey.cols+j] << std::endl;
            
            if (max<R_value[i*prey.cols+j])
            {
                max = R_value[i*prey.cols+j];
            }
        }
    for (int i = patch_H/2 ; i < prey.rows-patch_H/2 ; i++)
        for (int j = patch_W/2 ; j < prey.cols-patch_W/2 ; j++)
        {
            R_value[i*prey.cols+j] /= max;
            R_value[i*prey.cols+j] *= 255;
            Min_Eigen_Value[i*prey.cols+j] /= 1.0*max_min;
            Min_Eigen_Value[i*prey.cols+j] *= 255;
            Max_Eigen_Value[i*prey.cols+j] /= 1.0*max_max;
            Max_Eigen_Value[i*prey.cols+j] *= 255;
        }
    R_Threshold = 0.8;
}

bool ChenRJ_Detector::Local_Maximum(int x,int y)
{
    bool Local_Max = true;
    
    if (R_value[Get_Index(x,y)] < R_Threshold) Local_Max = false;
  
    for (int i = -4 ; i <= 4 ; i++)
        for (int j = -4 ; j <= 4 ; j++)
            
            if ((Get_Index(x+i,y+j)>=0) && (Get_Index(x+i,y+j) < prey.cols*prey.rows) && (R_value[Get_Index(x,y)] < R_value[Get_Index(x+i,y+j)])) Local_Max = false;
   
    return Local_Max;
}

int ChenRJ_Detector::Get_Index(int x,int y)
{
    return x*prey.cols+y;
}
