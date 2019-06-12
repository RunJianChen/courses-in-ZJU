//
//  ChenRJ_EigenFace.cpp
//  ChenRJ_EigenFace
//
//  Created by Chen~_~RJ on 2018/12/10.
//  Copyright © 2018 Chen~_~RJ. All rights reserved.
//

#include "ChenRJ_EigenFace.hpp"

ChenRJ_EigenFace::ChenRJ_EigenFace(vector<Mat> *Training_data,double Energy_Rate)
{
    Origin_Data = *Training_data;
    Subspace_Dimension = Max_Featured_Face*Energy_Rate;
    Common_H = H;
    Common_W = W;
    Transform_to_Subspace = Mat(Subspace_Dimension,Common_H*Common_W,CV_32FC1);
    Database.resize(Origin_Data.size());
    
    for (int i = 0 ; i < Database.size() ; i++)
    {
        Database[i] = Mat(Subspace_Dimension,1,CV_32FC1);
    }
    Train();
}

void ChenRJ_EigenFace::Train()
{
    Pre_Process();
    
    vector<float> *Average_Face = new vector<float>(Common_H*Common_W);

    Calculate_Average(Average_Face);//calculate average face
    
    Calculate_Trans_Matrix(Subspace_Dimension,Average_Face);//transform matrix calculator
    
}

void ChenRJ_EigenFace::Calculate_Average(vector<float> *Average_Face)
{
    for (int j = 0 ; j < H ; j++)
        for (int k = 0 ; k < W ; k++)
        {
            (*Average_Face)[Get_Index(j,k)] = 0 ;
        }
    for (int i = 0 ; i < Origin_Data.size() ; i++)
    {
        for (int j = 0 ; j < H ; j++)
            for (int k = 0 ; k < W ; k++)
            {
                (*Average_Face)[Get_Index(j,k)] += static_cast<float>(Origin_Data[i].ptr<unsigned char>(j)[k*3])+static_cast<float>(Origin_Data[i].ptr<unsigned char>(j)[k*3+1])+static_cast<float>(Origin_Data[i].ptr<unsigned char>(j)[k*3+2]);
                
            }
    }
    for (int j = 0 ; j < H ; j++)
        for (int k = 0 ; k < W ; k++)
        {
            (*Average_Face)[Get_Index(j,k)] = (*Average_Face)[Get_Index(j,k)]/Origin_Data.size();
            (*Average_Face)[Get_Index(j,k)] = (*Average_Face)[Get_Index(j,k)]/3.0;
        }
}

int ChenRJ_EigenFace::Get_Index(int Height, int Width)
{
    if ((Height<0) || (Height>=H) || (Width<0) || (Width>=W))
        return -1;
    else
        return Height*W+Width;
}

void ChenRJ_EigenFace::Pre_Process()
{
    
}


Mat ChenRJ_EigenFace::Get_Face_from_Database(int Index)
{
    Mat Face_Demanded;
    Face_Demanded = Origin_Data[Index];
    return Face_Demanded;
}

int ChenRJ_EigenFace::Indentification(Mat New_Face)
{
    int Index = -1 ;
    double min = 100000000;
    
    Mat now = Mat(Common_W*Common_H,1,CV_32FC1);
    
    for (int j = 0 ; j < Common_H ; j++)
        for (int k = 0 ; k < Common_W ; k++)
        {
            now.ptr<float>(j*Common_W+k)[0] = static_cast<float>(New_Face.ptr<unsigned char>(j)[k*3])-im_Avg.ptr<float>(j)[k];
        }
    
    Mat Data = Mat(Subspace_Dimension,1,CV_32FC1);
    Data = Transform_to_Subspace*now;
    
    for (int i = 0 ; i < Origin_Data.size() ; i++)
    {
        if (norm(Data,Database[i]) < min)
        {
            Index = i;
            min = norm(Data,Database[i]);
        }
    }
    //cout <<min << endl;
    return Index;
}

Mat ChenRJ_EigenFace::Reconstruct(Mat Reconstruct_Source,double chosen_rate)
{
    Mat New_Face,temp2;
    
    Mat now = Mat(Common_W*Common_H,1,CV_32FC1);
    
    for (int j = 0 ; j < Common_H ; j++)
        for (int k = 0 ; k < Common_W ; k++)
        {
            now.ptr<float>(j*Common_W+k)[0] = static_cast<float>(Reconstruct_Source.ptr<unsigned char>(j)[k*3])-im_Avg.ptr<float>(j)[k];
        }
    
    Mat Data = Mat(Subspace_Dimension,1,CV_32FC1);
    
    Data = Transform_to_Subspace*now;
    
    int chosen_num = Subspace_Dimension*chosen_rate;
    
    Mat chosen_faces = Mat(chosen_num,1,CV_32FC1);
    
    for (int i = 0 ; i < chosen_num ; i++)
    {
        chosen_faces.ptr<float>(i)[0] = Data.ptr<float>(i)[0];
    }
    
    temp2 = Mat(Common_H,Common_W,CV_32FC1);
    for (int j = 0 ; j < Common_H ; j++)
        for (int k = 0 ; k < Common_W ; k++)
        {
            temp2.ptr<float>(j)[k] = 0;
        }
    
    for (int i = 0 ; i < chosen_num ; i++)
    {
        Mat row_n = Transform_to_Subspace.row(i),row_now;
        normalize(Transform_to_Subspace.row(i),row_now,0,255,NORM_MINMAX);
        
        for (int j = 0 ; j < Common_H ; j++)
            for (int k = 0 ; k < Common_W ; k++)
            {
                temp2.ptr<float>(j)[k] = temp2.ptr<float>(j)[k]+chosen_faces.ptr<float>(i)[0]*((row_now).ptr<float>(0)[j*Common_W+k]);
            }
    }
    
    temp2 += im_Avg;
    
    normalize(temp2,New_Face,0,255,NORM_MINMAX);
    
    return New_Face;
}

void ChenRJ_EigenFace::Calculate_Trans_Matrix(int Dimension, vector<float> *Average_Face)
{
    Mat Cov = Mat(Common_H*Common_W,Origin_Data.size(),CV_32FC1);
    //matrix fine
    
    im_Avg = Mat(H,W,CV_32FC1);
    //average image
    
    //get average image
    for (int j = 0 ; j < Common_H ; j++)
        for (int k = 0 ; k < Common_W ; k++)
        {
            float * pos = im_Avg.ptr<float>(j);
            pos[k] =  static_cast<float>((*Average_Face)[j*Common_W+k]);
        }
    
    //calculate matrix fine
    for (int i = 0 ; i < Origin_Data.size() ; i++)
    {
        for (int j = 0 ; j < Common_H ; j++)
            for (int k = 0 ; k < Common_W ; k++)
            {
                float * pos = im_Avg.ptr<float>(j);
                Cov.ptr<float>(j*Common_W+k)[i] = (static_cast<float>(Origin_Data[i].ptr<unsigned char>(j)[k*3])+static_cast<float>(Origin_Data[i].ptr<unsigned char>(j)[k*3+1])+static_cast<float>(Origin_Data[i].ptr<unsigned char>(j)[k*3+2]))/3 - pos[k];
            }
    }
    
    //calculate eigen vectors and values
    
    Mat C = Cov.t()*Cov;
    Mat eValuesMat;
    Mat eVectorsMat;

    eigen(C, eValuesMat, eVectorsMat);
    
    //select eigen vectors
    
    vector<Mat> Eigen_Vectors = *new vector<Mat>(Subspace_Dimension);
    
    for (int i = 0 ; i < Subspace_Dimension ; i++)
    {
        Eigen_Vectors[i] = Mat(Origin_Data.size(),1,CV_32FC1);
        
        Eigen_Vectors[i] = eVectorsMat.row(i).t();
        
    }
    
    //calculate transform matrix
    
    for (int i = 0 ; i < Subspace_Dimension ; i++)
    {
        Mat now = Mat(Common_W*Common_H,1,CV_32FC1);
        now = (Cov*Eigen_Vectors[i]);
        normalize(now.t(),Transform_to_Subspace.row(i),1,0,NORM_L1);
    }
    
    //generate database
    for (int i = 0 ; i < Origin_Data.size() ; i++)
    {
        Mat now = Mat(Common_W*Common_H,1,CV_32FC1);
        for (int j = 0 ; j < Common_H ; j++)
            for (int k = 0 ; k < Common_W ; k++)
            {
                now.ptr<float>(j*Common_W+k)[0] = static_cast<float>(Origin_Data[i].ptr<unsigned char>(j)[k*3])-im_Avg.ptr<float>(j)[k];;
            }
        Database[i] = Transform_to_Subspace*now;
    }
    
}
