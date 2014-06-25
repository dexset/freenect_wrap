#include <stdio.h>
#include <device.h>
#include <vector>
#include <unistd.h>

int main()
{
    Freenect::Freenect freenect;
    BasicFreenectDevice** device;
    unsigned int num_dev = freenect.deviceCount();
    if( num_dev < 1 )
    {
        fprintf( stderr, "No devices connected\n" );
        return -1;
    }
    fprintf( stderr, "Number of devices found:%d\n", num_dev );
    device = new BasicFreenectDevice*[num_dev];
    for( int i = 0; i < num_dev; i++ )
    {
        device[i] = &freenect.createDevice<BasicFreenectDevice>(i);
        device[i]->startVideo();
        device[i]->startDepth();
        fprintf( stderr, "Started RGB for device %d\n", i );
        fprintf( stderr, "Started depth for device %d\n", i );
    }

    uint16_t* depth_buffer = new uint16_t[ 640 * 480 ];
    uint8_t*  rgb_buffer = new uint8_t[ 640 * 480 * 3 ];
	while(true)
    {
        for( int i = 0; i < num_dev; i++ )
        {
            device[i]->updateState();
            bool rgb_state = device[i]->getRGB( rgb_buffer );
            if( rgb_state )
                printf( "%d got rgb\n", i );
            bool depth_state = device[i]->getDepth( depth_buffer );
            if( depth_state )
                printf( "%d got depth\n", i );
        }
        usleep( 1000 );
    }
    for( int i = 0; i < num_dev; i++ )
    {
        device[i]->stopVideo();
        device[i]->stopDepth();
    }
    printf( "Deinit" );
    return 0;
}
