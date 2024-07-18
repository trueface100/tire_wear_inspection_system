#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/kernels/register.h"
using namespace std;
using namespace cv;

#include <chrono>   


int main(){

    // Variables to measure invoke latency.
    int num_test=100;
    struct timespec begin, end;
    double latency = 0;
    int pred_0=0;
    int pred_1=0;

    ////////////////////////////////////////////
    // Load the TensorFlow Lite model
    const char* model_path = "../tireD_lenet3.tflite";  // Replace with the actual path to your .tflite file
    std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile(model_path);

    if (!model) {
        std::cerr << "Error loading the TensorFlow Lite model." << std::endl;
        return -1;
    }

  // Create an interpreter
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder builder(*model, resolver);
    std::unique_ptr<tflite::Interpreter> interpreter;
    builder(&interpreter);

    if (!interpreter) {
        std::cerr << "Error creating the TensorFlow Lite interpreter." << std::endl;
        return -1;
    }

    // Allocate tensors
    if (interpreter->AllocateTensors() != kTfLiteOk) {
        std::cerr << "Error allocating tensors." << std::endl;
        return -1;
    }


   

    // Assuming input tensor index is 0
    int input_tensor_index = 0;
    TfLiteTensor* input_tensor = interpreter->tensor(input_tensor_index);
    int input_index = interpreter->inputs()[0];
    int output_index = interpreter->outputs()[0];

    TfLiteIntArray* input_dims = interpreter->tensor(input_index)->dims;

    for(int seq=0; seq<num_test; seq++){
        string filename = "/home/user3057/workspace/project/test_data/test_bad/canny/test_bad_" + to_string(seq) + ".jpg";
        // string filename = "./test_bad/test_" + to_string(seq) + ".jpg";
        cv::Mat img_edges = imread(filename, cv::IMREAD_GRAYSCALE);
        cv::resize(img_edges, img_edges, cv::Size(640,480));
        cv::Mat input_tensor(img_edges);
        input_tensor.convertTo(input_tensor, CV_32F, 1.0 / 255.0);
        interpreter->tensor(input_index)->data.f = input_tensor.ptr<float>(0);



        clock_gettime(CLOCK_MONOTONIC, &begin);

        if (interpreter->Invoke() != kTfLiteOk) {
            std::cerr << "Error running inference." << std::endl;
            return -1;
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        double temp = (end.tv_sec - begin.tv_sec) + ((end.tv_nsec - begin.tv_nsec) / 1000000000.0);
        latency += temp;

        // Get the output tensor
        TfLiteTensor* output_tensor = interpreter->output_tensor(0);
        float* output_data = output_tensor->data.f;

        // Post-process the output
        int predicted_label = std::distance(output_data, std::max_element(output_data, output_data + output_tensor->dims->data[output_tensor->dims->size - 1]));

    



        if (predicted_label == 0) {
            std::cout << "The model predicts: Good" << std::endl;
            pred_0 +=1;
        } else {
            std::cout << "The model predicts: Bad" << std::endl;
            pred_1 +=1;
        }
       
        
  
        printf("pred_0 is %d\n", pred_0);
        printf("pred_1 is %d\n", pred_1);
    }

    printf("=========================================\n");
    printf("Test for 100 test images labeled as 'bad' \n");

    printf("%d samples had been predicted as 'good' \n", pred_0);
    printf("%d samples had been predicted as 'bad' \n", pred_1);

    printf("Total elepsed time : %.6f sec\n", latency);
    printf("Average inference latency : %.6f sec\n", latency / num_test);

    return 0;
}