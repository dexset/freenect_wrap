#include <device.h>

Freenect::Freenect freenect;

BasicFreenectDevice** device;

extern "C"
{
    unsigned int initializeFreenectDevices()
    {
        unsigned int num_dev = freenect.deviceCount();
        if( num_dev < 1 )
            return 0;

        device = new BasicFreenectDevice*[num_dev];
        for( int i = 0; i < num_dev; i++ )
        {
            device[i] = &freenect.createDevice<BasicFreenectDevice>(i);
            device[i]->startVideo();
            device[i]->startDepth();
        }
        return num_dev;
    }

    void deinitializeFreenectDevices()
    {
        for( int i = 0; i < freenect.deviceCount(); i++ )
        {
            device[i]->stopVideo();
            device[i]->stopDepth();
        }
    }

    void processFreenectDevices()
    {
        for( int i = 0; i < freenect.deviceCount(); i++ )
            device[i]->updateState();
    }

    bool getRGB( unsigned int devID, uint8_t* buffer )
    { device[devID]->getRGB( buffer ); }

    bool getDepth( unsigned int devID, uint16_t* buffer )
    { device[devID]->getDepth( buffer ); }

    unsigned int getRGBWidth() { return MED_RES_W; }
    unsigned int getRGBHeight() { return MED_RES_H; }

    unsigned int getDepthWidth() { return MED_RES_W; }
    unsigned int getDepthHeight() { return MED_RES_H; }
}
