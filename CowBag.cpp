// CowBag.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <librealsense2/rs.hpp>
#include "windows.h"

using namespace cv;

int x_size = 500, y_size = 500;
bool array1[500][500];
bool array2[500][500];
bool array_ref[500][500];
int draw_bias_x = x_size / 2;
int draw_bias_y = y_size / 2;
int X4 = 0, Y4 = 0;
int t = 200, x = 0, y = 0, k = 0;
int max = 0, a = 0, c = 0, d = 0, i = 0;
float b = 0;
int MCx, MCy, MC2x, MC2y;
int MCx_t = 1, MCy_t = 1, MC2x_t = 1, MC2y_t = 1;
int MCrefx_t, MCrefy_t, MCrefx, MCrefy;
int error1, error2, foo, FIRST = 0;
int ML, MLmin = x_size/16, obj_y_end, ANDsum, XORsum, Cross, Cross1, Cross2, Cross3, dc_est, dd_est, oc, od, dc, dd;

#define POINT_COLOR Scalar(0,10,255)

void MyLine(Mat img, Point start, Point end)
{
    int thickness = 1;
    int lineType = LINE_8;
    line(img,
        start,
        end,
        Scalar(50, 10, 255),
        thickness,
        lineType);
}
void drawPoint(Mat img, Point point2d)
{
    int thickness = 1;
    int lineType = LINE_8;
    line(img,
        point2d,
        point2d,
        POINT_COLOR,
        thickness,
        lineType);
}
void drawPointColored(Mat img, Point point2d, Scalar color)
{
    int thickness = 1;
    int lineType = LINE_8;
    line(img,
        point2d,
        point2d,
        color,
        thickness,
        lineType);
}
int func1(int x, int y, float R);
int CC(int start_from_y);
int RIGHT(int pointsr);
int LEFT(int points);
int DOWN(int points);
int UP(int points);
void setup();
void draw(Mat img);



int main()
{
    rs2::pipeline pipe;
    rs2::config cfg;
    rs2::colorizer colorizer;

    Mat img(600, 800, CV_8UC3, Scalar(100, 250, 30));
    String windowName = "Wii";
    String cameraWindowName = "cameraWindow";
    namedWindow(windowName);
    namedWindow(cameraWindowName);

    //cfg.enable_stream(RS2_STREAM_COLOR);
    cfg.enable_stream(RS2_STREAM_DEPTH, 1280, 720, RS2_FORMAT_Z16, 30);
    pipe.start(cfg);
    rs2::frameset frameset;

    while (1) {
        img.setTo(Scalar(0, 0, 0));
        draw(img);
        imshow(windowName, img);


        for (int ui = 0; ui < 2; ui++) {
            frameset = pipe.wait_for_frames();
        }
        //rs2::frameset frameset = pipe.wait_for_frames();
        rs2::frame depth = frameset.get_depth_frame();
        auto colorized_depth = colorizer.colorize(depth);

        if (depth) {
            Mat depthmap(Size(1280, 720), CV_8UC3, (void*)colorized_depth.get_data(), Mat::AUTO_STEP);
            imshow(cameraWindowName, depthmap);
        }

        Sleep(10);
        if (waitKey(40) != -1) {
            destroyWindow(windowName);
            destroyWindow(cameraWindowName);
            pipe.stop();
            return 0;

        }
    }


} 

void camera_stream_start(rs2::config cfg, rs2::pipeline pipe) {

    cfg.enable_stream(RS2_STREAM_COLOR);
    //cfg.enable_stream(RS@_STREAM_DEPTH);
    pipe.start(cfg);

}
rs2::frameset get_frameset(rs2::pipeline pipe) {
    //rs2::frameset frameset_t = pipe.wait_for_frames();
    return (pipe.wait_for_frames());
}



int func1(int x, int y, float R) {
    x -= draw_bias_x;
    return ((int)(y - 0.01f * x * x - 10 * cosf(((float)x) / 10 + R * M_PI) - 0 * sinf(((float)x) / 160 + R * M_PI / 80)));
}
int CC(int start_from_y) {
    int k = start_from_y;
    ANDsum = 0; XORsum = 0;
    for (x = 0; x < x_size; x++) {
        if ((array1[x][y] == array2[x][y]) && (array2[x][y] == true)) ANDsum += 1;
        if (array1[x][y] != array2[x][y]) XORsum += 1;
    }
    if (ANDsum > XORsum)return (ANDsum - XORsum);
    else return 0;
}
int RIGHT(int pointsr) {
    for (int xx = (x_size - 1); xx >= 0; xx--) {
        for (int yy = 0; yy < y_size; yy++) {
            if ((xx - pointsr) >= 0)array2[xx][yy] = array2[(xx - pointsr)][yy];
        }
    }
    return pointsr;
}
int LEFT(int points) {
    for (int x = 0; x < x_size; x++) {
        for (int y = 0; y < y_size; y++) {
            if ((x - points) >= 0)array2[(x - points)][y] = array2[x][y];
        }
    }
    return points;
}
int DOWN(int points) {
    for (int x = 0; x < x_size; x++) {
        for (int y = y_size; y > 0; y--) {
            if ((y + points) < y_size)array2[x][y + points] = array2[x][y];
            if (y < points)array2[x][y] = array2[x][y + 1];//drawing shifted leg down in area
        }
    }
    return points;
}
int UP(int points) {
    for (int x = 0; x < x_size; x++) {
        for (int y = 0; y < y_size; y++) {
            if ((y - y_size) >= 0)array2[x][y - points] = array2[x][y];
        }
    }
    return points;
}



void setup() {
    MLmin = (int)(x_size / 16);

}
void draw(Mat img) {

    c = (int)((rand() % 64) - 32 + 250);
    d = (int)((rand() % 64) - 32 + 250);
    X4 = c;
    Y4 = d;
    MCx = 0; MCx_t = 1; MCy = 0; MCy_t = 1;
    b = (float)(rand()%1000)/1000;
    //--contour draw-----------------
    
    for (x = 0; x < 500; x++) {
        //point(x+X4-draw_bias_x, Y4+func1((x),0,b));
        drawPointColored(img, Point(x + X4 - draw_bias_x, Y4 + func1((x), 0, b)), Scalar(255, 255, 255));
    }
    


    //-------------------
    for (x = 0; x < x_size; x++) {
        for (y = 0; y < y_size; y++) {
            if (y <= (Y4 + func1((x - X4 + draw_bias_x), 0, b))) {
                array1[x][y] = true;
                MCx += x; MCy += y;
                MCx_t += 1; MCy_t += 1;
            }
            else {
                array1[x][y] = false;
            }
        }
    }
    if (MCx_t > 1)MCx_t -= 1;
    MCx /= MCx_t;
    if (MCy_t > 1)MCy_t -= 1;
    MCy /= MCy_t;
    dc = c - oc; dd = d - od;// Случайный сдвиг нового объекта относительно опорного. dc - по горизонтали; dd - по вертикали;
    //--------------------------
    if (FIRST == 0) {
        MCrefx = 0; MCrefx_t = 1; MCrefy = 0; MCrefy_t = 1;
        oc = c; od = d;
        for (x = 0; x < x_size; x++) {
            for (y = 0; y < y_size; y++) {
                if (y <= (Y4 + func1((x - X4 + draw_bias_x), 0, 0))) {
                    array_ref[x][y] = true;
                    MCrefx += x; MCrefy += y;
                    MCrefx_t += 1; MCrefy_t += 1;
                }
                else {
                    array_ref[x][y] = false;
                }
            }
        }

        if (MCrefx_t > 1)MCrefx_t -= 1;
        MCrefx /= MCrefx_t;
        if (MCrefy_t > 1)MCrefy_t -= 1;
        MCrefy /= MCrefy_t;
        FIRST = 1;
    }
    //-----------------------------
    MC2x = MCrefx; MC2x_t = 1; MC2y = MCrefy; MC2y_t = 1;
    for (x = 0; x < x_size; x++) {
        for (y = 0; y < y_size; y++) {
            array2[x][y] = array_ref[x][y];
        }
    }
    dc_est = (MCx - MC2x); dd_est = (MCy - MC2y);
    if (MC2x < MCx)RIGHT(MCx - MC2x);//shift x slice of array2 to right direction 
    if (MC2x > MCx)LEFT(MC2x - MCx);//shift x slice of array2 to left direction
    if (MC2y < MCy)DOWN(MCy - MC2y);
    if (MC2y > MCy)UP(MC2y - MCy);
    //--------------------------------
    MC2x = 0; MC2y = 0; MC2x_t = 1; MC2y_t = 1;
    for (x = 0; x < x_size; x++) {
        for (y = 0; y < y_size; y++) {
            if (array2[x][y] == true) {
                MC2x += x; MC2y += y;
                MC2x_t += 1; MC2y_t += 1;
            }
        }
    }
    if (MC2x_t > 1)MC2x_t -= 1;
    MC2x /= MC2x_t;
    if (MC2y_t > 1)MC2y_t -= 1;
    MC2y /= MC2y_t;
    error1 = (int)(sqrtf((MC2x-MCx)*(MC2x-MCx)+(MC2y-MCy)*(MC2y-MCy)));
    //-----------------------
    if (error1 > 1) {
        Cross1 = CC(MC2y); UP(16); dd_est -= 16;
        Cross2 = CC(MC2y);
        if (Cross2 < Cross1) {
            DOWN(32); dd_est += 32;
        }
        Cross3 = CC(MC2y);
        if (Cross3 < Cross1) {
            UP(16);
            dd_est -= 16;
        }

        Cross1 = CC(MC2y); UP(8); dd_est -= 8;
        Cross2 = CC(MC2y);
        if (Cross2 < Cross1) {
            DOWN(16); dd_est += 16;
        }
        Cross3 = CC(MC2y);
        if (Cross3 < Cross1) {
            UP(8);
            dd_est -= 8;
        }

        Cross1 = CC(MC2y); UP(4); dd_est -= 4;
        Cross2 = CC(MC2y);
        if (Cross2 < Cross1) {
            DOWN(8); dd_est += 8;
        }
        Cross3 = CC(MC2y);
        if (Cross3 < Cross1) {
            UP(4);
            dd_est -= 4;
        }

        Cross1 = CC(MC2y); UP(2); dd_est -= 2;
        Cross2 = CC(MC2y);
        if (Cross2 < Cross1) {
            DOWN(4); dd_est += 4;
        }
        Cross3 = CC(MC2y);
        if (Cross3 < Cross1) {
            UP(2);
            dd_est -= 2;
        }

        Cross1 = CC(MC2y); UP(1); dd_est -= 1;
        Cross2 = CC(MC2y);
        if (Cross2 < Cross1) {
            DOWN(2); dd_est += 2;
        }
        Cross3 = CC(MC2y);
        if (Cross3 < Cross1) {
            UP(1);
            dd_est -= 1;
        }
    }//error >1
    MC2x = 0; MC2y = 0; MC2x_t = 1; MC2y_t = 1;
    for (x = 0; x < x_size; x++) {
        for (y = 0; y < y_size; y++) {
            if ((array2[x][y] == true) && (x % 4 == 2)) {
                //drawPoint(img, Point(x, y));
                drawPointColored(img, Point(x, y), Scalar(0, 0, 255));
            }
            if ((array1[x][y] == true) && (x % 4 == 3)) {
                //drawPoint(img, Point(x, y));
                drawPointColored(img, Point(x, y), Scalar(255, 255, 255));
            }
            if (array2[x][y] == true) {
                MC2x += x; MC2y += y;
                MC2x_t += 1; MC2y_t += 1;
            }
        }
    }
    if (MC2x_t > 1)MC2x_t -= 1;
    MC2x /= MC2x_t;
    if (MC2y_t > 1)MC2y_t -= 1;
    MC2y /= MC2y_t;
    error1 = (int)(sqrtf((MC2x-MCx)*(MC2x-MCx)+(MC2y-MCy)*(MC2y-MCy)));
    error2 = (int)(sqrtf((dc_est-dc)*(dc_est-dc)+(dd_est-dd)*(dd_est-dd)));

    //--------------------------
    obj_y_end = MC2y;
    for (y = MC2y; y < y_size; y++) {
        ML = 0;
        for (x = 0; x < x_size; x++) {
            if (array2[x][y] == true) {
                ML++;
            }
        }
        if (ML < MLmin) {
            obj_y_end = y;
            y = y_size;
        }
    }
    MyLine(img, Point(10, obj_y_end), Point((x_size - 10), obj_y_end));
    MyLine(img, Point(x_size - 10, obj_y_end), Point((x_size - 10), (y_size)));
    MyLine(img, Point(x_size - 10, y_size), Point(10, y_size));
    MyLine(img, Point(10, y_size), Point(10, obj_y_end));


    Cross = CC(0);

    putText(img, format("Y4 =     %d",Y4), Point(420, 410), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1);
    putText(img, format("X4 =     %d", X4), Point(420, 430), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1);
    putText(img, format("MCx =    %d", MCx), Point(420, 450), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1);
    putText(img, format("MCy =    %d", MCy), Point(420, 470), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1);
    putText(img, format("MC2x =   %d", MC2x), Point(320, 450), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1);
    putText(img, format("MC2y =   %d", MC2y), Point(320, 470), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1);
    putText(img, format("MCrefx = %d", MCrefx), Point(220, 450), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1);
    putText(img, format("MCrefy = %d", MCrefy), Point(220, 470), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1);
    putText(img, format("error  = %d", error1), Point(420, 490), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1);
    putText(img, format("error2 = %d", error2), Point(220, 490), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1);

    putText(img, format("dc_est = %d", dc_est), Point(20, 490), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1);
    putText(img, format("dd_est = %d", dd_est), Point(120, 490), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1);
    putText(img, format("dc     = %d", dc), Point(20, 470), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1);
    putText(img, format("dd     = %d", dd), Point(120, 470), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1);
    putText(img, format("Cross  = %d", Cross), Point(320, 490), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1);

    //putText(img, format("b  = %f", b), Point(50, 540), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1);

}


