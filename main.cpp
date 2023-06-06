#include <iostream>
#include <GxIAPI.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    // Initialize daheng web cameras.
    GX_STATUS status = GX_STATUS_SUCCESS;
    GX_STATUS camStatus[2] = {GX_STATUS_SUCCESS, GX_STATUS_SUCCESS};

    // Calls GXInitLib() at the start location to initialize resources.
    status = GXInitLib();
    if (status != GX_STATUS_SUCCESS)
    {
        cout << "Initialize DAHENG camera's library fail!" << endl;
        return status;
    }

    // Update device list.
    uint32_t nDeviceNum = 0;
    status = GXUpdateDeviceList(&nDeviceNum, 1000);
    GX_DEVICE_IP_INFO stIPInfo[2];
    if (status == GX_STATUS_SUCCESS && nDeviceNum == 2)
    {
        // Gets the network informations.
        camStatus[0] = GXGetDeviceIPInfo(1, &stIPInfo[0]);
        camStatus[1] = GXGetDeviceIPInfo(2, &stIPInfo[1]);
    }
    else
    {
        cout << "Update device list fail!" << endl;
        return status;
    }

    // Open camera.
    GX_DEV_HANDLE hDevice[2] = {NULL, NULL};
    // Open the first device in the enumeration list via ip.
    status = GXOpenDeviceByIndex(1, &hDevice[0]);
    // Setting device’s property of GevSCPSPacketSize to improve the //acquisition performance of the network camera.
    bool bImplementPacketSize = false;
    status = GXSetInt(hDevice[0], GX_INT_GEV_PACKETSIZE, 1500);
    // Open the second device in the enumeration list via ip.
    // stOpenParam.pszContent = stIPInfo[1].szIP;
    // status = GXOpenDevice(&stOpenParam, &hDevice[1])

    // Accquire image.
    PGX_FRAME_BUFFER pFrameBuffer[2];
    // Stream On.
    status = GXStreamOn(hDevice[0]);
    // status = GXStreamOn(hDevice[1]);

    if (status == GX_STATUS_SUCCESS)
    {
        status = GXDQBuf(hDevice[0], &pFrameBuffer[0], 1000);
        if (pFrameBuffer[0]->nStatus == GX_FRAME_STATUS_SUCCESS)
        {
            Mat frame = Mat(1024, 1280, CV_8UC1, pFrameBuffer[0]->pImgBuf);
            imwrite("/home/ff/git/smart_sensor/build/a.bmp", frame);
        }
        status = GXQBuf(hDevice[0], pFrameBuffer[0]);
    }
    status = GXStreamOff(hDevice[0]);

    // Closes the Device.
    status = GXCloseDevice(hDevice[0]);
    // status = GXCloseDevice(hDevice[1]);
    // Calls GXCLoseLib() at the end of the program to release resources.
    status = GXCloseLib();
    return 0;
}