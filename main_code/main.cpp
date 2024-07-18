#include "lcd.h"
#include <opencv2/opencv.hpp>
#include "ultrasonic.h"
#include "rgb.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/kernels/register.h"
#include "servo.h"
#include <softTone.h>
#define SPKR 7
using namespace std;


int fd;  // seen by all subroutines
int red_pin=2;
int green_pin=3;


char array1[] = "Tire Worn Out!";
char array2[] = "Buy New One";

int main()   {

  

  //lcd
  fd = wiringPiI2CSetup(I2C_ADDR);
  lcd_init(); // setup LCD
  
  

  printf("HC-SR04 Ultra-sonic distance measure program \n");
  if (wiringPiSetup() == -1) {
      exit(EXIT_FAILURE);
  }
  rgb_init(red_pin,green_pin);

  //backlight
  pinMode(15,OUTPUT);
  digitalWrite(15,LOW);
  



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

  while (shouldCapture) {
    
    car_distance = Echo_Test();
    if(car_distance<=5){
      digitalWrite(15,HIGH);
      delay(3000);
    }
    else continue;
    if (shouldCapture && Echo_Test() <= 5) {
        
        // 사진 캡쳐

        cv::Mat image;
        cap >> image;

        if (!image.empty()) {
            cv::imwrite("car.jpg", image);
            std::cout << "사진을 car.jpg로 저장했습니다." << std::endl;
            
        } else {
            std::cerr << "프레임을 읽을 수 없습니다." << std::endl;
        }
        digitalWrite(15,LOW);
        shouldCapture = false; // 사진을 찍었으므로 shouldCapture를 false로 설정
    } else if (!shouldCapture && car_distance > 5) {
        shouldCapture = true; // 거리가 5cm를 초과하면 다시 사진을 찍을 준비
    }

  }

  cap.release();
  //////
  ///
  // 이미지를 로드합니다.
  cv::Mat image = cv::imread("car.jpg");
  
  // 로드한 이미지가 비어 있는지 확인합니다.
  if (image.empty()) {
      std::cerr << "이미지를 불러오는데 실패했습니다." << std::endl;
      return -1;
  }
  
  // 흑백 이미지로 변환합니다.
  cv::Mat grayImage;
  cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
  
  // Canny Edge Detection을 적용합니다.
  cv::Mat img_edges;
  cv::Canny(grayImage, img_edges, 30, 100); // 여기서 50과 150은 각각 최소값과 최대 임계값입니다.

  // 결과를 저장합니다.
  cv::imwrite("car_canny.jpg", img_edges);
  ///
  ////////////////////////////////////////////
  // Load the TensorFlow Lite model
    const char* model_path = "./tireD_lenet2.tflite";  // Replace with the actual path to your .tflite file
    std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile(model_path);

    if (!model) {
        std::cerr << "Error loading the TensorFlow Lite model." << std::endl;
        return -1;
    }

    // Create an interpreter
    tflite::ops::builtin::BuiltinOpResolver resolver;

    std::unique_ptr<tflite::Interpreter> interpreter;
    tflite::InterpreterBuilder(*model, resolver)(&interpreter);

    if (!interpreter) {
        std::cerr << "Error creating the TensorFlow Lite interpreter." << std::endl;
        return -1;
    }

    // Allocate tensors
    if (interpreter->AllocateTensors() != kTfLiteOk) {
        std::cerr << "Error allocating tensors." << std::endl;
        return -1;
    }

    // Get input and output details
    int input_index = interpreter->inputs()[0];
    int output_index = interpreter->outputs()[0];
    TfLiteIntArray* input_dims = interpreter->tensor(input_index)->dims;
  ///
    std::cout<<"input_dims->data[2]:"<<input_dims->data[2]<<std::endl;
    std::cout<<"input_dims->data[1]"<<input_dims->data[1]<<std::endl;
    cv::resize(img_edges, img_edges, cv::Size(input_dims->data[2], input_dims->data[1]));
    // cv::imshow("input image",img_edges);
    // cv::waitKey(0);
    cv::Mat input_tensor(img_edges);

     // Normalize pixel values to the range [0, 1]
    input_tensor.convertTo(input_tensor, CV_32F, 1.0 / 255.0);

    // Reshape the input tensor
    interpreter->tensor(input_index)->data.f = input_tensor.ptr<float>(0);

    // Run inference
    if (interpreter->Invoke() != kTfLiteOk) {
        std::cerr << "Error running inference." << std::endl;
        return -1;
    }

    // Get the output tensor
    TfLiteTensor* output_tensor = interpreter->output_tensor(0);
    float* output_data = output_tensor->data.f;

    // Post-process the output
    int predicted_label = std::distance(output_data, std::max_element(output_data, output_data + output_tensor->dims->data[output_tensor->dims->size - 1]));

    // Print the result
    if (predicted_label == 0) {
        std::cout << "The model predicts: Good" << std::endl;
    } else {
        std::cout << "The model predicts: Bad" << std::endl;
    }




  bool is_safe= !predicted_label;

  if(!is_safe){

    

    softToneCreate(SPKR);
    for (int i = 0; i < 3; i++) {
      softToneWrite(SPKR, 1000);
      delay(100);
      softToneWrite(SPKR, 0);
      delay(50);
    }

    for(int i=0; i<5; i++){
      ledPwmControl(red_pin);
      // cout <<"lcd displaying"<< endl;
      ClrLcd();
      lcdLoc(LINE1);
      typeln(array1);
      lcdLoc(LINE2);
      typeln(array2);
      // delay(1);
    }
    
    
  }
  else{
    for(int i=0; i<10; i++){
      ledPwmControl(green_pin);
      ClrLcd();
      lcdLoc(LINE1);
      typeln("Your Tire is");
      lcdLoc(LINE2);
      typeln("Fine!!");
    }
  }
  rgb_init(red_pin,green_pin);//turn off
  // delay(2000);

  ///servo
  int servo_pin=23;


  servo_init(servo_pin);
  rotateServo(servo_pin, 90);
  delay(3000);
  rotateServo(servo_pin,180);
  ClrLcd();





  return 0;

}
