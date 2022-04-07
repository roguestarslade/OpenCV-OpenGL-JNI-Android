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

void tflite_obj:: tflite_model(pt_camera_info info, InputOutputArray img, std::vector<Point2f> getPoints) {

    __android_log_print(ANDROID_LOG_DEBUG, APPNAME, "tflite_model");

    // Fill input buffers
    // TODO(user): Insert code to fill input tensors.
    // Note: The buffer of the input tensor with index `i` of type T can
    float* input = interpreter->typed_input_tensor<float>(0);

    //check size here
    memcpy(input, &img, info.res_x*info.res_y*sizeof(float));

    // Run inference
    TFLITE_MINIMAL_CHECK(interpreter->Invoke() == kTfLiteOk);
    tflite::PrintInterpreterState(interpreter.get());

    float* output = interpreter->typed_output_tensor<float>(0);
    std::ostringstream cout;
    cout << output[0];
    cout << "\n-------\n";
    __android_log_print(ANDROID_LOG_DEBUG, APPNAME, "%s", cout.str().c_str());
}