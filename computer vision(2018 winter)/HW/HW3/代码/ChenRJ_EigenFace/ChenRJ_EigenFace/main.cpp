//
//  main.cpp
//  ChenRJ_EigenFace
//
//  Created by Chen~_~RJ on 2018/12/10.
//  Copyright © 2018 Chen~_~RJ. All rights reserved.
//

#include <iostream>
#include "ChenRJ_EigenFace.hpp"

int main(int argc, const char * argv[])
{
    int num_of_people = 41;
   
    vector<Mat> *data = new vector<Mat>(num_of_people*9);
    
    string Path = "/Users/chenrj/Desktop/视觉/att_faces/s",filename,extend = ".pgm",now;
    
    for (int i = 0 ; i < num_of_people ; i++)
    {
        for (int j = 0 ; j < 9 ; j++)
        {
            filename = Path;
            now = to_string(i+1);
            filename += now + "/";
            filename += to_string(j+2) + extend;
            (*data)[(i)*9+j] = imread(filename);
        }
    }
    
    ChenRJ_EigenFace new_database(data,1);
    
    
    //reconstruct test
    /*
    filename = "/Users/chenrj/Desktop/视觉/att_faces/s41/1.pgm";
    Mat Source = imread(filename);
    Mat re = new_database.Reconstruct(Source,1);
    imwrite("/Users/chenrj/Desktop/1.jpg",re);
    */
    
    
    //identification test
    
    filename = "/Users/chenrj/Desktop/视觉/att_faces/s41/1.pgm";
    Mat Source = imread(filename);
    int Index = new_database.Indentification(Source);
    
    string str = to_string((Index+1)/9+1);
    Point ID_pos(40,40);
    putText(Source, str, ID_pos,CV_FONT_HERSHEY_COMPLEX, 0.5, CvScalar(0,0,255));
    
    imwrite("/Users/chenrj/Desktop/1.jpg",Source);
    imwrite("/Users/chenrj/Desktop/2.jpg",new_database.Get_Face_from_Database(Index));
    
    
}
