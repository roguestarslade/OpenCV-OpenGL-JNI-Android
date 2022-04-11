//
// Created by rangkast.jeong on 2022-04-07.
//

#ifndef CONTROLLER_TFLITE_OBJ_H
#define CONTROLLER_TFLITE_OBJ_H

#include "utils.h"

#define TFLITE_MINIMAL_CHECK(x)                                  \
    if (!(x))                                                    \
    {                                                            \
        fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__); \
        exit(1);                                                 \
    }

using namespace cv;
class tflite_obj {
public:
    void tflite_load_model(pt_camera_info info);
    void tflite_model(pt_camera_info info, Mat &matInput, std::vector<Point2f> getPoints);
};
#endif //CONTROLLER_TFLITE_OBJ_H