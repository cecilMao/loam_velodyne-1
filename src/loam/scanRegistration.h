#pragma once

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ros/ros.h>

#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/PointCloud2.h>

#include <tf/transform_datatypes.h>
#include <tf/transform_broadcaster.h>

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/kdtree/kdtree_flann.h>

namespace scanRegistration
{

#define CLOUD 2100000 // cloudSortInd and cloudSortInd
#define PICKSMALL 1000
class scanRegistration
{
public:
    const double maxRangeFeaturesCorners = 150; // originally 30
    const double minRangeFeaturesCorners = 1.3; // originally 0.3
    const double maxRangeFeaturesFlat = 150; // originally 30
    const double minRangeFeaturesFlat = 1.3; // originally 0.3
    const int cornerPoints = 1000;
    const int cornerPointsLess = 4000;
    const int surfPoins = 1000; // PICKSMALL
    scanRegistration(ros::Publisher * pubLaserCloudExtreCur, ros::Publisher * pubLaserCloudLast);
    const double PI = 3.1415926;
    const double rad2deg = 180 / PI;
    const double deg2rad = PI / 180;

    double initTime;

    bool systemInited = false;

    double timeScanCur = 0;
    double timeScanLast = 0;

    int laserRotDir = 1;

    int skipFrameNum = 2;
    int skipFrameCount = 0;

    pcl::PointCloud<pcl::PointXYZHSV>::Ptr laserCloudExtreCur;//(new pcl::PointCloud<pcl::PointXYZHSV>());
    pcl::PointCloud<pcl::PointXYZHSV>::Ptr laserCloudLessExtreCur;//(new pcl::PointCloud<pcl::PointXYZHSV>());

    sensor_msgs::PointCloud2 laserCloudExtreCur2;
    sensor_msgs::PointCloud2 laserCloudLast2;

    pcl::PointCloud<pcl::PointXYZHSV>::Ptr outLaserCloudExtreCur2;
    pcl::PointCloud<pcl::PointXYZHSV>::Ptr outLaserCloudLast2;

    ros::Publisher* pubLaserCloudExtreCurPointer;
    ros::Publisher* pubLaserCloudLastPointer;

    int imuPointerFront = 0;
    int imuPointerLast = -1;
    static const int imuQueLength = 50;
    bool imuInited = false;

    float imuRollStart, imuPitchStart, imuYawStart;
    float imuRollCur, imuPitchCur, imuYawCur;

    float imuVeloXStart, imuVeloYStart, imuVeloZStart;
    float imuShiftXStart, imuShiftYStart, imuShiftZStart;
    float imuVeloXCur, imuVeloYCur, imuVeloZCur;
    float imuShiftXCur, imuShiftYCur, imuShiftZCur;

    float imuShiftFromStartXCur, imuShiftFromStartYCur, imuShiftFromStartZCur;
    float imuVeloFromStartXCur, imuVeloFromStartYCur, imuVeloFromStartZCur;

    double imuTime[imuQueLength] = {0};
    float imuRoll[imuQueLength] = {0};
    float imuPitch[imuQueLength] = {0};
    float imuYaw[imuQueLength] = {0};

    float imuAccX[imuQueLength] = {0};
    float imuAccY[imuQueLength] = {0};
    float imuAccZ[imuQueLength] = {0};

    float imuVeloX[imuQueLength] = {0};
    float imuVeloY[imuQueLength] = {0};
    float imuVeloZ[imuQueLength] = {0};

    float imuShiftX[imuQueLength] = {0};
    float imuShiftY[imuQueLength] = {0};
    float imuShiftZ[imuQueLength] = {0};

    //double imuAccuRoll = 0;
    //double imuAccuPitch = 0;
    double imuAccuYaw = 0;

    void ShiftToStartIMU();
    void VeloToStartIMU();
    void TransformToStartIMU(pcl::PointXYZHSV *p);
    void AccumulateIMUShift();
    void laserCloudHandler(const sensor_msgs::PointCloud2ConstPtr& laserCloudIn2);
    void laserCloudHandlerVelo(const pcl::PointCloud<pcl::PointXYZ>::Ptr laserCloudIn, pcl::PointCloud<pcl::PointXYZHSV>::Ptr cornerPointsSharp, pcl::PointCloud<pcl::PointXYZHSV>::Ptr cornerPointsLessSharp, pcl::PointCloud<pcl::PointXYZHSV>::Ptr surfPointsFlat, pcl::PointCloud<pcl::PointXYZHSV>::Ptr surfPointsLessFlatDS, bool pubExtre=false, bool pubLast=false);
    void imuHandler(const sensor_msgs::Imu::ConstPtr& imuIn);

private:
    double timeStart;
    double timeLasted;

    int cloudSortInd[CLOUD];
    int cloudNeighborPicked[CLOUD];

    int createInsidePC(const pcl::PointCloud<pcl::PointXYZ>::Ptr laserCloudIn, pcl::PointCloud<pcl::PointXYZHSV>::Ptr laserCloud);
    float calcLaserAngle(const pcl::PointXYZ laserPointFirst, const pcl::PointXYZ inLaserPointLast);
    void computeSmoothness(pcl::PointCloud<pcl::PointXYZHSV>::Ptr laserCloud, int cloudSize);
    void setImuTrans(pcl::PointCloud<pcl::PointXYZHSV>::Ptr imuTrans);
    bool isNewSweep(pcl::PointCloud<pcl::PointXYZ>::Ptr laserCloudIn);
    void unknownFunction(pcl::PointCloud<pcl::PointXYZHSV>::Ptr laserCloud, int cloudSize);
    void compFeatures(pcl::PointCloud<pcl::PointXYZHSV>::Ptr cornerPointsSharp, pcl::PointCloud<pcl::PointXYZHSV>::Ptr cornerPointsLessSharp, pcl::PointCloud<pcl::PointXYZHSV>::Ptr surfPointsFlat, pcl::PointCloud<pcl::PointXYZHSV>::Ptr surfPointsLessFlat, pcl::PointCloud<pcl::PointXYZHSV>::Ptr laserCloud, int cloudSize);
    void haveSweep();
    void unknownImuStuff(pcl::PointCloud<pcl::PointXYZHSV>::Ptr laserCloud, int cloudSize);
};

}
