#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    // 命令行参数：可指定摄像头ID和背景图路径
    int camId = 0;
    vector<string> bgPaths;

    if (argc > 1) camId = atoi(argv[1]);
    if (argc > 2) {
        for (int i = 2; i < argc; i++)
            bgPaths.push_back(argv[i]);
    } else {
        // 默认背景
        bgPaths.push_back("C:/Users/ccs/Desktop/code/project_hide/test01/video_screenshot_09.05.2026.png");
    }

    // 加载所有背景图
    vector<Mat> bkds;
    for (const auto& path : bgPaths) {
        Mat img = imread(path);
        if (!img.empty())
            bkds.push_back(img);
        else
            cerr << "Warning: failed to load " << path << endl;
    }
    if (bkds.empty()) {
        cerr << "No valid background images" << endl;
        return -1;
    }

    int bgIndex = 0;
    Mat bkd = bkds[0];

    Mat frame;
    VideoCapture capture(camId);
    if (!capture.isOpened()) {
        cerr << "Cannot open camera " << camId << endl;
        return -1;
    }

    // 创建调参窗口和 trackbar
    namedWindow("invisible");

    int h_low = 0, s_low = 0, v_low = 0;
    int h_high = 180, s_high = 255, v_high = 60;
    int morph_size = 3;   // 形态学核大小，0=关闭
    int blur_size = 15;   // 高斯模糊核大小，0=关闭

    createTrackbar("H low", "invisible", &h_low, 180);
    createTrackbar("S low", "invisible", &s_low, 255);
    createTrackbar("V low", "invisible", &v_low, 255);
    createTrackbar("H high", "invisible", &h_high, 180);
    createTrackbar("S high", "invisible", &s_high, 255);
    createTrackbar("V high", "invisible", &v_high, 255);
    createTrackbar("morph ksize", "invisible", &morph_size, 21);
    createTrackbar("blur ksize", "invisible", &blur_size, 41);

    while (capture.read(frame))
    {
        // 如果背景尺寸和摄像头不一致，缩放背景
        if (bkd.size() != frame.size())
            resize(bkd, bkd, frame.size());

        Mat hsv;
        cvtColor(frame, hsv, COLOR_BGR2HSV);

        // 颜色阈值分割
        Mat mask;
        inRange(hsv, Scalar(h_low, s_low, v_low), Scalar(h_high, s_high, v_high), mask);

        // 形态学开运算：先腐蚀再膨胀，消除孤立噪点
        if (morph_size > 0) {
            Mat kernel = getStructuringElement(MORPH_ELLIPSE,
                                               Size(morph_size, morph_size));
            morphologyEx(mask, mask, MORPH_OPEN, kernel);
        }

        // 高斯模糊：柔化mask边缘，消除合成时的硬边伪影
        int bs = blur_size;
        if (bs % 2 == 0) bs++;  // 高斯核必须为奇数
        if (bs > 0)
            GaussianBlur(mask, mask, Size(bs, bs), 0);

        Mat umask;
        bitwise_not(mask, umask);

        Mat blackToBck, personToShow;
        bitwise_and(bkd, bkd, blackToBck, mask);
        bitwise_and(frame, frame, personToShow, umask);

        Mat result;
        add(blackToBck, personToShow, result);

        imshow("invisible", result);

        int key = waitKey(30);
        if (key == 27 || cv::getWindowProperty("invisible", WND_PROP_VISIBLE) < 1)
            break;

        // 数字键 1-9 切换背景
        int numKey = key - '0';
        if (numKey >= 1 && numKey <= (int)bkds.size()) {
            bgIndex = numKey - 1;
            bkd = bkds[bgIndex];
        }
    }

    return 0;
}
