#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <opencv2/opencv.hpp> // OpenCV 헤더 추가

int TRIG = 29; // gpio 21 pin 40
int ECHO = 28; // gpio 20 pin 38

static int Echo_Test()
{
	unsigned long TX_time = 0;
	unsigned long RX_time = 0;
	float distance = 0;
	unsigned long timeout = 50000000; // 0.5 sec ~ 171 m   50*10^6 us
	unsigned long Wait_time = micros();
	pinMode(TRIG, OUTPUT);    //gpio 21 pin 40 using trigger
	pinMode(ECHO, INPUT);    //gpio 20 pin 38 using Echo ultra sound
	sr04.c41
		// Ensure trigger is low.
		digitalWrite(TRIG, LOW);
	delay(50);       //mili sec
	// Trigger tx sound.
	digitalWrite(TRIG, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG, LOW);
	// Wait for tx response, or timeout.
	while ((digitalRead(ECHO) == LOW && (micros() - Wait_time) < timeout)) {
		if (digitalRead(ECHO) == HIGH) break;
	}
	// Cancel on timeout.
	if ((micros() - Wait_time) > timeout) {
		printf("0 Out of range.micros =%d wait-time=%d \n", micros(), Wait_time);
		//return 0;
	}
	TX_time = micros();   //since call wiringPiSetup, the number of microseconds
	// Wait for rx sound response, or timeout.
	while ((digitalRead(ECHO) == HIGH && (micros() - Wait_time)) < timeout) {
		if (digitalRead(ECHO) == LOW) break;
	}
	// Cancel on timeout.
	if ((micros() - Wait_time) > timeout) {
		printf("1.Out of range.\n");
		//return 0;
	}
	RX_time = micros();
	// Calculate rx-tx duration to change distance.
	distance = (float)(RX_time - TX_time) * 0.017; //( 340m/2) *100cm/10^6 us
	printf("Range %.2f cm.\n", distance);
	return distance;
}

int main()
{
    printf("HC-SR04 Ultra-sonic distance measure program \n");
    if (wiringPiSetup() == -1) {
        exit(EXIT_FAILURE);
    }
    if (setuid(getuid()) < 0) {
        perror("Dropping privileges failed.\n");
        exit(EXIT_FAILURE);
    }

    cv::VideoCapture cap(0); // 카메라 초기화
    if (!cap.isOpened()) {
        std::cerr << "카메라를 열 수 없습니다." << std::endl;
        return -1;
    }

    bool shouldCapture = true; // 사진을 찍어야 하는지 여부를 결정하는 플래그
    int car_distance = 999;

    while (true) {
        car_distance = Echo_Test();
        if (shouldCapture && car_distance <= 5) {
            // 사진 캡쳐
            cv::Mat image;
            cap >> image;

            if (!image.empty()) {
                cv::imwrite("car.jpg", image);
                std::cout << "사진을 car.jpg로 저장했습니다." << std::endl;
            } else {
                std::cerr << "프레임을 읽을 수 없습니다." << std::endl;
            }
            shouldCapture = false; // 사진을 찍었으므로 shouldCapture를 false로 설정
        } else if (!shouldCapture && car_distance > 5) {
            shouldCapture = true; // 거리가 5cm를 초과하면 다시 사진을 찍을 준비
        }
        delay(500); // 500ms 지연
    }

    cap.release();
    return 0;
}
