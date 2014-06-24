#include <stdio.h>
#include <device.h>
#include <vector>

int main()
{
    Freenect::Freenect freenect;
    BasicFreenectDevice* device;
	device = &freenect.createDevice<BasicFreenectDevice>(0);
	device->startVideo();
    printf( "Started RGB\n" );
	device->startDepth();
    printf( "Started depth\n" );
    std::vector<uint16_t> depth_buffer;
    std::vector<uint8_t>  rgb_buffer;
    depth_buffer.resize( 640 * 480 * 4 );
    rgb_buffer.resize( 640 * 480 * 4 );
	while(true)
    {
        device->updateState();
        //printf( "rgb_state:  %d\n", device->getRGB( rgb_buffer ) );
        //printf( "depth_state:%d\n", device->getDepth( depth_buffer ) );
        //device->getRGB( rgb_buffer );
        //device->getDepth( depth_buffer );
    }
	device->stopVideo();
	device->stopDepth();
    printf( "Deinit" );
    return 0;
}
