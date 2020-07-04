 void _Rec()
 {
    imwrite("f:\\imgCanny.jpg",imgCanny);
    imgROIs.clear();
    imgROICannys.clear();
    points.clear();
    Mat imgCannyNew;
    imgCanny.copyTo(imgCannyNew);
    //将新的图复制到img，不使用imgOri
    //copy new image to img in order to not using imgOri
    Mat img;
    imgOri.copyTo(img);
    vector<vector<Point>> contours;
    //findContourts会改变imgCanny的值
    //Because findContours will change the value of imgCanny
    findContours(imgCanny, contours, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    //轮廓的数量 contours
    //contours means the number of outlines
    vector<RotatedRect> boxs;
    for (int i = 0;i < contours.size();i++) {
        RotatedRect box = minAreaRect(contours[i]);
        Point2f vertex[4];
        box.points(vertex);
        boxs.push_back(box);
        for (int j = 0;j < 4;j++) {
            line(img, vertex[j], vertex[(j + 1) % 4], Scalar(0, 0, 255), 2);
        }
    }

    //取中点
    //get the middle point
    vector<Point2f>center;
    vector<Point>pointAll;		//所有的旋转矩形四个点
    for (int i = 0;i < boxs.size();i++) {
        Point2f vertex[4];
        boxs[i].points(vertex);
        int x_sum(0), y_sum(0);
        for (int j = 0;j < 4;j++) {
            x_sum = vertex[j].x + x_sum;
            y_sum = vertex[j].y + y_sum;
            pointAll.push_back(Point(vertex[j].x, vertex[j].y));
        }
        center.push_back(Point2f((float)(x_sum / 4), (float)(y_sum / 4)));
    }
    Mat imgTmp1;
    //cvtColor(img,imgTmp1,CV_BGR2RGB);
    img.copyTo(imgTmp1);
    for(auto tmp:center){
        circle(imgTmp1,tmp,4,Scalar(225,225,0),2);
    }
    imwrite("f:\\imgCenter.jpg",imgTmp1);

    //换算比例尺
    //先取四个角的点，然后矫正
    //calculating the scale
    //get the position of four corner points and correct of distortion

    Point TopLeft;
    Point TopRight;
    Point BottomRight;
    Point BottomLeft;
    vector<Point> center_max, center_min;	//定义center点中比较大和比较小的点集
    for (int i = 0;i < center.size();i++) {
        if (center[i].y > img.rows / 2) {
            center_max.push_back(center[i]);
        }
        if (center[i].y < img.rows / 2) {
            center_min.push_back(center[i]);
        }
}

    //寻找center较大和较小值中最大最小x
    int x_max(0), x_min(10000);
    for (int i = 0;i < center_max.size();i++) {
        if (center_max[i].x < x_min) {
            x_min = center_max[i].x;
            BottomLeft = center_max[i];
        }
        if (center_max[i].x > x_max) {
            x_max = center_max[i].x;
            BottomRight = center_max[i];
        }
    }
    x_max = 0, x_min = 10000;
    for (int i = 0;i < center_min.size();i++) {
        if (center_min[i].x < x_min) {
            x_min = center_min[i].x;
            TopLeft = center_min[i];
        }
        if (center_min[i].x > x_max) {
            x_max = center_min[i].x;
            TopRight = center_min[i];
        }
    }

    circle(img, TopLeft, 5, Scalar(255, 255, 0), 2);
    circle(img, TopRight, 5, Scalar(255, 255, 0), 2);
    circle(img, BottomLeft, 5, Scalar(255, 255, 0), 2);
    circle(img, BottomRight, 5, Scalar(255, 255, 0), 2);


    //在center点集中去掉四个定位点
    for (int i = 0;i < center.size();i++) {
        if (PointDistance(TopLeft, center[i]) < 2) {
            center.erase(center.begin() + i);
            break;
        }
    }
    for (int i = 0;i < center.size();i++) {
        if (PointDistance(TopRight, center[i]) < 2) {
            center.erase(center.begin() + i);
            break;
        }
    }
    for (int i = 0;i < center.size();i++) {
        if (PointDistance(BottomLeft, center[i]) < 2) {
            center.erase(center.begin() + i);
            break;
        }
    }
    for (int i = 0;i < center.size();i++) {
        if (PointDistance(BottomRight, center[i]) < 2) {
            center.erase(center.begin() + i);
            break;
        }
    }

    for (int i = 0;i < center.size();i++) {
        circle(img, center[i], 3, Scalar(0, 255, 255), 1);
    }

    qDebug() << "识别数量" << center.size() ;
    imwrite("f:\\imgResult.jpg",img);
    ui->reslut->setText("识别数量："+QString::number(center.size())+"个物体");
    cvtColor(img, img, CV_BGR2RGB);
    ui->reslutPicture1->setPixmap(QPixmap::fromImage(QImage(
                                                 img.data,
                                                 img.cols,
                                                 img.rows,
                                                 img.cols*img.channels(),
                                                 QImage::Format_RGB888)));


    for(int i=0; i<center.size();i++)
    {
        string num = std::to_string(i+1);
        cv::putText(img, num, center[i], FONT_HERSHEY_PLAIN, 4.0, Scalar(0,255,0), 3);
    }

    ui->reslutPicture2->setPixmap(QPixmap::fromImage(QImage(
                                                  img.data,
                                                  img.cols,
                                                  img.rows,
                                                  img.cols*img.channels(),
                                                  QImage::Format_RGB888)));
    ui->reslutPicture3->setPixmap(QPixmap::fromImage(QImage(
                                                  img.data,
                                                  img.cols,
                                                  img.rows,
                                                  img.cols*img.channels(),
                                                  QImage::Format_RGB888)));
    ui->reslutPicture4->setPixmap(QPixmap::fromImage(QImage(
                                                  img.data,
                                                  img.cols,
                                                  img.rows,
                                                  img.cols*img.channels(),
                                                  QImage::Format_RGB888)));
    vector<int> Hs;
    FindPointH(img, center, Hs);
    QString colorText = "识别结果：\n";
    for(int i=0; i<center.size(); i++)
    {
        colorText.append("物体"+QString::number(i+1)+"颜色为:"+QString::number(Hs[i])+"\n");
    }
    ui->colorResult->setText(colorText);

    //获取图像ROI，存入imgROIs中
    for (int i = 0;i < center.size();i++) {
        for (int j = 0;j < boxs.size();j++) {
            if (DoesRectangleContainPoint(boxs[j], center[i])) {
                Point2f vertex[4];
                boxs[j].points(vertex);
                int x_max(0), y_max(0), x_min(10000), y_min(10000);
                for (int t = 0;t < 4;t++) {
                    if (vertex[t].x > x_max)
                        x_max = vertex[t].x;
                    if (vertex[t].y > y_max)
                        y_max = vertex[t].y;
                    if (vertex[t].x < x_min)
                        x_min = vertex[t].x;
                    if (vertex[t].y < y_min)
                        y_min = vertex[t].y;
                }
                imgROIs.push_back(imgOri(Rect(x_min, y_min, x_max - x_min, y_max - y_min)));
                imgROICannys.push_back(imgCannyNew(Rect(x_min-1, y_min-1, (x_max - x_min)+2, (y_max - y_min)+2)));
            }
        }
    }


    //矫正畸变，并重新定位各个中心点
    //以左上和右下为准
    Point newBottomLeft = Point(TopLeft.x, BottomRight.y);
    Point newTopRight = Point(BottomRight.x, TopLeft.y);
    //制作输入输出点数组,并计算变换矩阵
    Point2f point_src[4], point_dst[4];
    point_src[0] = TopLeft;
    point_src[1] = TopRight;
    point_src[2] = BottomRight;
    point_src[3] = BottomLeft;
    point_dst[0] = TopLeft;
    point_dst[1] = newTopRight;
    point_dst[2] = BottomRight;
    point_dst[3] = newBottomLeft;
    Mat M = getPerspectiveTransform(point_src, point_dst);

    vector<Point2f> newCenter;
    perspectiveTransform(center, newCenter, M);

    //计算比例尺rate
    double real = sqrt(pow(LOCATION_WIDTH, 2) + pow(LOCATION_HEIGHT, 2));
    double px = sqrt(pow((BottomRight.x - TopLeft.x), 2) + pow((BottomRight.y - TopLeft.y), 2));
    double rate = real / px;

    Mat T = Mat::zeros(Size(3, 3), CV_32F);
    T.at<float>(0, 2) = newBottomLeft.y + (LOCATION_CENTER * (1 / rate));
    T.at<float>(1, 2) = -(newBottomLeft.x + ((BottomRight.x - newBottomLeft.x) / 2));
    T.at<float>(2, 2) = 1;
    T.at<float>(0, 1) = -1;
    T.at<float>(1, 0) = 1;

    //将每个center点转换为Mat矩阵,newCenter为实际坐标中的点
    vector<Mat> centers, newCenters;
    for (int i = 0;i < newCenter.size();i++) {
        Mat tem = Mat::zeros(3, 1, CV_32F);
        tem.at<float>(0, 0) = newCenter[i].x;
        tem.at<float>(1, 0) = newCenter[i].y;
        tem.at<float>(2, 0) = 1;
        centers.push_back(tem);
    }
    for (int i = 0;i < centers.size();i++) {
        newCenters.push_back(T*centers[i] * rate);
    }


    for (int i = 0;i < newCenters.size(); i++) {
        Point t = Point(newCenters[i].at<float>(0, 0), newCenters[i].at<float>(1, 0));
        points.push_back(t);
    }
    for(auto tmp:points){
        qDebug() << "("+QString::number(tmp.x)+","+QString::number(tmp.y)+")";
    }
}


void MainWindow::on_ArduinoCode_clicked()
{
    ArduinoLanguageGlobal.clear();
    vector<float> M1;
    vector<float> distance;
    for (int i = 0;i < points.size();i++) {
        if (points[i].y == 0) {
            M1.push_back(90);
            distance.push_back(PointDistance(points[i], Point(0, 0)));
            continue;
        }
        if (points[i].y < 0) {
            M1.push_back(RtoA(atan((float)points[i].x / (-points[i].y))));
            distance.push_back(PointDistance(points[i], Point(0, 0)));
            continue;
        }
        distance.push_back(PointDistance(points[i], Point(0, 0)));
        M1.push_back(180 - RtoA(atan((float)points[i].x / points[i].y)));
    }
    //计算抓取角度
    vector<float> M2, M3, M4;
    const float a1(78), a2(120), a3(125), a4(193);
    for (int i = 0;i < distance.size();i++) {
        float k = distance[i] - a3;
        float phi = acos(-k / sqrt(pow(k, 2) + pow(a1, 2)));
        qDebug() << "phi = " << phi;
        float x = ((float)pow(a4, 2) - pow(k, 2) - pow(a1, 2) - pow(a2, 2)) / (2 * a2*sqrt(pow(k, 2) + pow(a1, 2)));
        float seita1 = pi - asin(x) - phi;
        float seita3 = acos(((float)distance[i] - a3 - a2 * sin(seita1)) / a4);
        M2.push_back(90 - (RtoA(seita1)));
        M3.push_back(RtoA(seita1));
        M4.push_back(90 - RtoA(seita3));
    }

    //检查是否有角度超过90，如果有，就把这个角度设为90
    for (int i = 0;i < M2.size();i++) {
        if (M2[i] > 90)
            M2[i] = 90;
        if (M3[i] > 90)
            M3[i] = 90;
        if (M4[i] > 90)
            M4[i] = 90;
    }
    //矫正角度
    for(int i=0; i<M1.size(); i++)
    {
        float rate = (float)1/810;
        float M2t = -rate*M2[i]*(M2[i]-180)-10;
        if(M2t+M2[i]<0)
        {
            M2[i] = 0;
        }else{
            M2[i] = M2[i]+M2t;
        }
        ArduinoBuChang(M3[i],M3[i]);
        ArduinoBuChang(M4[i],M4[i]);
    }
    //生成Arduino语言

    QString ArduinoLanguage = "#include <Braccio.h>\n"
                      "#include <Servo.h>\n"
                      "Servo base;\n"
                      "Servo shoulder;\n"
                      "Servo elbow;\n"
                      "Servo wrist_rot;\n"
                      "Servo wrist_ver;\n"
                      "Servo gripper;\n"
                      "void setup() {\n"
                      "Braccio.begin();\n}\n"
                      "void loop() {\n"
                      "delay(5000);\n";

    for(int i=0;i<M3.size();i++){
ArduinoLanguage.append("Braccio.ServoMovement(20,"+QString::number(M1[i])+","+QString::number(M2[i])+","+QString::number(M3[i])+","+QString::number(M4[i])+","+QString::number(0)+","+QString::number(20)+");\n");
        ArduinoLanguage.append("delay(1000);\n");
        ArduinoLanguage.append("Braccio.ServoMovement(20,"+QString::number(M1[i])+","+QString::number(M2[i])+","+QString::number(M3[i])+","+QString::number(M4[i])+","+QString::number(0)+","+QString::number(73)+");\n");
        ArduinoLanguage.append("delay(1000);\n");
        ArduinoLanguage.append("Braccio.ServoMovement(20,"+QString::number(M1[i])+","+QString::number(M2[i])+","+QString::number(M3[i])+","+QString::number(M4[i])+","+QString::number(0)+","+QString::number(73)+");\n");
        ArduinoLanguage.append("delay(1000);\n");
        ArduinoLanguage.append("Braccio.ServoMovement(20,"+QString::number(M1[i])+","+QString::number(M2[i])+","+QString::number(M3[i])+","+QString::number(M4[i])+","+QString::number(0)+","+QString::number(20)+");\n");
        ArduinoLanguage.append("delay(1000);\n");
        ArduinoLanguage.append("Braccio.ServoMovement(20,"+QString::number(M1[i])+","+QString::number(M2[i])+","+QString::number(M3[i])+","+QString::number(M4[i])+","+QString::number(0)+","+QString::number(20)+");\n");
        ArduinoLanguage.append("delay(1000);\n");
    }
    ArduinoLanguage.append("}");
    ArduinoLanguageGlobal = ArduinoLanguage;
    ui->ArduinoText->setText(ArduinoLanguage);
}
