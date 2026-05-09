#include <iostream>
#include<opencv2/opencv.hpp> //引入头文件

using namespace cv;//命名空间
using namespace std;

//opencv机器视觉库，c/c++语言编写出来的，提供很多功能，都是以函数的形式提供
//我们只需要学会如何调用函数

int main(int argc, char *argv[])
{

    //1.显示图片
    Mat image = imread("d:/test.png");
    imshow("1",image);

    //2.转换成Hsv格式
    Mat hsv;
    cvtColor(image,hsv,COLOR_BGR2HSV);
    imshow("2",hsv);

    //3.截图颜色区域的范围 inRange  在蓝色的hsv取值范围内呈白色的抠图模板 ，不在范围内 呈黑色的抠图模板
    Mat mask;//抠图模板
    inRange(hsv,Scalar(100,43,46),Scalar(124,255,255),mask);
    imshow("3",mask);

    //4.现在可以蓝色背景表示出来了，但是实际上想要扣人物
    //取反操作
    bitwise_not(mask,mask);
    imshow("4",mask);

    //5.生成一张红色背景图 大小？ 类型？ 都是由原始图片 参考 参照
    //::是C++的作用域分解运算符 调用Mat对象里面的xxx函数
    //zeros创建一个指定行数、列数、类型的矩阵 像素都为0
    Mat redBack = Mat::zeros(image.size(),image.type());
    redBack = Scalar(40,40,200);
    imshow("5",redBack);

    //6.实现图片的拷贝操作
    image.copyTo(redBack,mask);
    imshow("6",redBack);

    waitKey(0);//等待用户按下任意键

    return 0;//表示程序正常结束
}


/*
    //1.把一张图片显示出来  ①找到那张图片 ②显示那张图片 \n \t 转义符
    Mat img = imread("d:/niu0.jpg");//Mat类型->图片类型  容器对象类型 变量img
    imshow("title",img);//显示图片

    //2.图片转模糊图片  -> 车牌识别 人脸识别 过程
    Mat blurImg;//模糊图片
    blur(img,blurImg,Size(50,50));//模糊处理方法  size（横向模糊尺寸，纵向模糊尺寸）
    imshow("blur",blurImg);

    //3.图片做灰色处理 - 数据量减少3倍 （以后业务跟颜色无关的，都可以先灰色处理）
    Mat grayImg;//灰色图片
    cvtColor(img,grayImg,COLOR_BGR2GRAY);//颜色处理函数
    imshow("gray",grayImg);

    //4.图片做二值化处理
    Mat threImg;//二值化图片
    threshold(grayImg,threImg,170,200,THRESH_BINARY);//二值化处理
    imshow("thre",threImg);

*/





