#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <iostream>
int main(int argc, const char** argv)
{
    cv::VideoCapture video (0);
    if (!video.isOpened())
{
    std::cout << "Unable to get video from the camera!" << std::endl;
    return -1;
}
cv::Mat frame;
while (video.read(frame))
{
    cv::imshow("Video feed", frame);
    if (cv::waitKey(25) >= 0)
    break;
}
cv::destroyAllWindows();
video.release();
return 0;
}

