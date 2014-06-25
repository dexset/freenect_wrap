module dwrap.freenect;

extern (C)
{
    uint initializeFreenectDevices();
    void deinitializeFreenectDevices();
    void processFreenectDevices();
    bool getRGB( uint, ubyte* ); //memory must be allocated
    bool getDepth( uint, ushort* ); //memory must be allocated

    uint getRGBWidth();
    uint getRGBHeight();

    uint getDepthWidth();
    uint getDepthHeight();
}
