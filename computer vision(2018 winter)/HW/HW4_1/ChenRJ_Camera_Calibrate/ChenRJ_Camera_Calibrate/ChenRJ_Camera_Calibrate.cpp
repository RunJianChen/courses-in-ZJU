//
//  ChenRJ_Camera_Calibrate.cpp
//  Camera_Calibrating
//
//  Created by Chen~_~RJ on 2018/12/25.
//  Copyright © 2018 Chen~_~RJ. All rights reserved.
//

#include "ChenRJ_Camera_Calibrate.hpp"

void ChenRJ_Camera_Calibrate::Calculate_Parameter()
{
    bool* Proper_data = new bool[Raw_Data.size()];
    vector<vector<Point3f>> object_points(1);
    int count = -1;
    Size Board_sz = Size(Board_Width,Board_Height);
    vector<vector<Point2f>> corners(0);
    
    Size win = Size(11,11),Zeros = Size(-1,-1);
    
    TermCriteria criteria = TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 );
    
    //preprocess
    for (int i = 0 ; i < Raw_Data.size() ; i++)
    {
        vector<Point2f> corner;
        
        bool corners_found ;

        corners_found = findChessboardCorners(Raw_Data[i],Board_sz,corner);

        if (!corners_found)
        {
            cout << "Error in processing " << i <<"th photo" << endl;
            Proper_data[i] = false;
            continue;
        }
        
        Proper_data[i] = true;
      
        Mat *now_im = new Mat(Raw_Data[i]), gray;
        
        cvtColor(*now_im , gray, CV_BGR2GRAY);
        
        count++;
        
        object_points[0].clear();
        
        float squareSize = 0.01;
        
        for( int i = 0; i < Board_Height; ++i )
            for( int j = 0; j < Board_Width; ++j )
                object_points[0].push_back(Point3f(float( j*squareSize ), float( i*squareSize ), 0));
        
        object_points.resize(Raw_Data.size(), object_points[0]);
    
        cornerSubPix( gray , corner , win , Zeros , criteria );
    
        corners.push_back(corner);
    }
    
    //caculate camera parameters
    Mat cameraMatrix,distCoeffs,rvecs, tvecs;
    
    calibrateCamera(object_points, corners, Board_sz, cameraMatrix, distCoeffs,rvecs, tvecs);
    
    Intrinsic_Parameter = cameraMatrix;
    Extrinsic_Parameter_t = tvecs;
    Extrinsic_Parameter_r = rvecs;
    Distorn_Parameter = distCoeffs;
}


Mat ChenRJ_Camera_Calibrate::Bird_View(Mat Source)
{
    Mat bird_view = Source;
    
    //undistort
    
    Size imageSize = Size(Raw_Data[0].cols,Raw_Data[0].rows);
    Mat map1 , map2;
    
    initUndistortRectifyMap(Intrinsic_Parameter, Distorn_Parameter, Mat(),
                            getOptimalNewCameraMatrix(Intrinsic_Parameter, Distorn_Parameter, imageSize, 1, imageSize, 0),
                            imageSize, CV_16SC2, map1, map2);
    
    remap(bird_view, bird_view, map1, map2, INTER_LINEAR);
    
    imwrite("/Users/chenrj/Desktop/2.jpg", bird_view);
    
    //find corners in source map
    
    Size Board_sz = Size(Board_Width,Board_Height);
    
    vector<Point2f> corner;
    
    bool corners_found ;
    
    Size win = Size(11,11),Zeros = Size(-1,-1);
    
    TermCriteria criteria = TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 );
    
    corners_found = findChessboardCorners(bird_view,Board_sz,corner);
    
    Mat bird_gray;
    
    cvtColor(bird_view , bird_gray, CV_BGR2GRAY);
    
    cornerSubPix( bird_gray , corner , win , Zeros , criteria );
    
    //tranform
    
    Point2f dst_points[] = {
        Point2f(400, 1200),
        Point2f(700, 1200),
        Point2f(400 ,1500),
        
        Point2f(700, 1500) };
    
    Point2f src_points[] = {
        corner[0],
        corner[Board_Width-1],
        corner[(Board_Height-1)*Board_Width],
        corner[(Board_Height-1)*Board_Width+Board_Width-1]};
    
    Mat M = getPerspectiveTransform(src_points, dst_points);
    
    warpPerspective(bird_view, bird_view, M, Size(Raw_Data[0].cols,Raw_Data[0].rows));
    
    
    return bird_view;
}
