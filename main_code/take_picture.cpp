#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

using namespace std;

int main(){
   

    if (wiringPiSetup() == -1) {
      exit(EXIT_FAILURE);
    }
    pinMode(15,OUTPUT);
    digitalWrite(15,HIGH);
    if (setuid(getuid()) < 0) {
    perror("Dropping privileges failed.\n");
    exit(EXIT_FAILURE);
    }

    
    cv::Mat image;
    cv::VideoCapture cap(0); // 카메라 초기화
    if (!cap.isOpened()) {
            std::cerr << "카메라를 열 수 없습니다." << std::endl;
            return -1;
    }

    for(int i=50; i<100; i++){
       
        
        cap >> image;
        std::string filename = "./wheel2/car_" + std::to_string(i) + ".jpg";

        if (!image.empty()) {
            cv::imwrite(filename, image);
            std::cout << "사진을 car.jpg로 저장했습니다." << std::endl;
            


        } else {
            std::cerr << "프레임을 읽을 수 없습니다." << std::endl;
        }

        
        delay(300);
        cv::Mat image2 = cv::imread(filename);

        // 로드한 이미지가 비어 있는지 확인합니다.
        if (image2.empty()) {
            std::cerr << "이미지를 불러오는데 실패했습니다." << std::endl;
            return -1;
        }
        
        // 흑백 이미지로 변환합니다.
        cv::Mat grayImage;
        cv::cvtColor(image2, grayImage, cv::COLOR_BGR2GRAY);
        
        // Canny Edge Detection을 적용합니다.
        cv::Mat edges;
        cv::Canny(grayImage, edges, 30, 100); // 여기서 50과 150은 각각 최소값과 최대 임계값입니다.

        // 결과를 저장합니다.
        std::string filename2 = "./wheel2/canny/car_"+ std::to_string(i) + ".jpg";
        cv::imwrite(filename2, edges);
        std::cout<<"picture"<<i<<"taken"<<std::endl;
        ///
        delay(1500);
        
    }
    cap.release();

    return 0;
}