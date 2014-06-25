module dwrap.freenect;

extern "C"
{
    uint initializeFreenectDevices();
    void deinitializeFreenectDevices();
    void processFreenectDevices();
    bool getRGB( unsigned int devID, ubyte* buffer ); //memory must be allocated
    bool getDepth( unsigned int devID, ushort* buffer ); //memory must be allocated

    uint getRGBWidth();
    uint getRGBHeight();

    uint getDepthWidth();
    uint getDepthHeight();
}
