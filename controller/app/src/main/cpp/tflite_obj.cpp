//
// Created by rangkast.jeong on 2022-04-07.
//

#include <iostream>
#include <cstdio>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/optional_debug_tools.h"
#include "opencv2/opencv.hpp"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <android/log.h>

#include "tflite_obj.h"

using namespace tflite;
using namespace cv;
using namespace std;

std::unique_ptr<FlatBufferModel> model;
tflite::ops::builtin::BuiltinOpResolver resolver;
std::unique_ptr<tflite::Interpreter> interpreter;

void tflite_obj:: tflite_load_model(pt_camera_info info)
{
    // Load model by using BuildFromFile
    /*
    std::unique_ptr<tflite::FlatBufferModel> model =
            tflite::FlatBufferModel::BuildFromFile(filename);
    */
    __android_log_print(ANDROID_LOG_DEBUG, APPNAME, "tflite_load_model :%p %d", info.tflite_model, info.tflite_model_size);
    model = tflite::FlatBufferModel::BuildFromBuffer(info.tflite_model, info.tflite_model_size);
    TFLITE_MINIMAL_CHECK(model != nullptr);

    // Build the interpreter with the InterpreterBuilder.
    // Note: all Interpreters should be built with the InterpreterBuilder,
    // which allocates memory for the Interpreter and does various set up
    // tasks so that the Interpreter can read the provided model.
    tflite::InterpreterBuilder builder(*model, resolver);
    builder(&interpreter);
    TFLITE_MINIMAL_CHECK(interpreter != nullptr);

    // Allocate tensor buffers.
    TFLITE_MINIMAL_CHECK(interpreter->AllocateTensors() == kTfLiteOk);
    printf("=== Pre-invoke Interpreter State ===\n");
    tflite::PrintInterpreterState(interpreter.get());
}

void tflite_obj:: tflite_model(pt_camera_info info, Mat &matInput, std::vector<Point2f> getPoints)
{
    // Fill input buffers
    // TODO(user): Insert code to fill input tensors.
    // Note: The buffer of the input tensor with index `i` of type T can
    float* input = interpreter->typed_input_tensor<float>(0);
    //check size here

    /*
    // creating a Tensor for storing the data
    tflite::Tensor input_tensor(DT_FLOAT, TensorShape({1,matInput.rows, matInput.cols,
                                                                       matInput.depth()}));
    auto input_tensor_mapped = input_tensor.tensor<float, 4>();


    // allocate a Tensor
    Tensor inputImg(Tensor::DT_FLOAT, TensorShape({1, matInput.rows, matInput.cols, 3}));
    // get pointer to memory for that Tensor
    float *p = inputImg.flat<float>().data();
    // create a "fake" cv::Mat from it
    cv::Mat cameraImg(matInput.rows, matInput.cols, CV_64FC1, p);
    // use it here as a destination
    cv::Mat imagePixels = matInput; // get data from your video pipeline
    imagePixels.convertTo(cameraImg, CV_64FC1);
*/

    void* object = (void*)matInput.ptr() ;

    //C++ API
    /*
    tflite::Tensor input_tensor(flatbuffers::ET_FLOAT, tensorflow::TensorShape({1,matInput.rows, matInput.cols,
                                                       matInput.depth()}));
    int data_size = matInput.rows * matInput.cols * matInput.depth() * sizeof(float);
    std::copy_n((float*)object, data_size, (input_tensor.flat<float>()).data());
*/

    //ToDo
    int data_size = matInput.rows * matInput.cols * matInput.depth() * sizeof(float);
    memcpy(input, object, data_size);
    // Run inference
    TFLITE_MINIMAL_CHECK(interpreter->Invoke() == kTfLiteOk);
    tflite::PrintInterpreterState(interpreter.get());

   // float* output = interpreter->typed_output_tensor<float>(0);
    int output_idx = interpreter->outputs()[0];
    float* output = interpreter->typed_output_tensor<float>(output_idx);

    std::ostringstream cout;
    cout << "OUTPUT: " << *output << std::endl;
    cout << "\n-------\n";

    __android_log_print(ANDROID_LOG_DEBUG, APPNAME, "%s", cout.str().c_str());

}
