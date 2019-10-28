#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;

std::vector<Mat> color_threshold(Mat &img){
    Mat hls_img,sobel_img, h_channel, l_channel, s_channel, thresh;
    std::vector<Mat> channels, returns;
    cvtColor(img, hls_img, COLOR_BGR2HLS);
    split(hls_img, channels);
    h_channel = channels[0];
    l_channel = channels[1];
    s_channel = channels[2];
    Sobel(s_channel, sobel_img,CV_8U, 1, 0,3);
    inRange(sobel_img,100, 255, thresh);
    Mat cropped = thresh(Rect(200,200,300,160));
    returns.push_back(cropped);
    returns.push_back(thresh);
    return returns;
}

Mat warpImage(Mat &image){
    Mat warped_img;
    Size s = image.size();
    int width = s.width;
    int height = s.height;

    Mat Mtx(2,4,CV_32FC1);
    Point2f src[4], dst[4];

    src[0] = Point2f(130,342);
    src[1] = Point2f(530,342);
    src[2] = Point2f(347,228);
    src[3] = Point2f(297,228);

    dst[0] = Point2f(130,360);
    dst[1] = Point2f(530,360);
    dst[2] = Point2f(530,0);
    dst[3] = Point2f(130,0);

    Mtx = getPerspectiveTransform(src,dst);
    warpPerspective(image, warped_img, Mtx, warped_img.size());

    return warped_img;
}

Mat histogram(Mat &image){
    // Use NumCPP to calculate histogram and sliding window.
    return hist;
}

int main(int argc, char *argv[])
{
    Mat frame, hls_img, cropped, thresh, warped_img;
    std::vector<Mat> images;
    namedWindow("Lanes", WINDOW_AUTOSIZE );
    namedWindow("Full Image", WINDOW_AUTOSIZE);
    namedWindow("Warped Image", WINDOW_AUTOSIZE);
    VideoCapture cap;
    cap.open("Lanes.mp4");
    int width = (int) cap.get(CAP_PROP_FRAME_WIDTH);
    int height = (int) cap.get(CAP_PROP_FRAME_HEIGHT);
    std::cout<< "Width: " << width << std::endl;
    std::cout << "Height: " << height << std::endl;
    while(1){
        cap >> frame;
        if(!frame.data) break;
        images = color_threshold(frame);
        cropped = images[0];
        thresh = images[1];
        warped_img = warpImage(thresh);
        Scalar hist = histogram(warped_img);
        std::cout << hist << std::endl;
        imshow("Lanes", cropped);
        imshow("Full Image", thresh);
        imshow("Warped Image", warped_img);
        char c = (char) waitKey(0);
        if (c=='q') break;
    }
    return 0;
}



