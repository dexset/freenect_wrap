#pragma once
#include "libfreenect.hpp"
#include "pthread.h"
#include "vector"
#include "stdio.h"
#include <cstring>

const unsigned int MED_RES_W = 640;
const unsigned int MED_RES_H = 480;

class Mutex 
{
private:
	pthread_mutex_t m_mutex;
public:
	Mutex() { pthread_mutex_init( &m_mutex, NULL ); }
	void lock() { pthread_mutex_lock( &m_mutex ); }
	void unlock() { pthread_mutex_unlock( &m_mutex ); }
    ~Mutex() { pthread_mutex_destroy( &m_mutex ); }

	class ScopedLock
	{
    private:
		Mutex & s_mutex;
	public:
		ScopedLock(Mutex & mutex): s_mutex(mutex)
		{ s_mutex.lock(); }
		~ScopedLock() { s_mutex.unlock(); }
	};
};

class BasicFreenectDevice : public Freenect::FreenectDevice 
{
private:
	std::vector<uint16_t> m_buffer_depth;
	std::vector<uint8_t> m_buffer_video;
	Mutex m_rgb_mutex;
	Mutex m_depth_mutex;
	bool m_new_rgb_frame;
	bool m_new_depth_frame;

public:
	BasicFreenectDevice(freenect_context *_ctx, int _index)
		: Freenect::FreenectDevice(_ctx, _index), 
        m_buffer_depth(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes),
        m_buffer_video(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes), 
        m_new_rgb_frame(false), m_new_depth_frame(false) {}

	void VideoCallback(void* _rgb, uint32_t timestamp) 
    {
		Mutex::ScopedLock lock(m_rgb_mutex);
		uint8_t* rgb = static_cast<uint8_t*>(_rgb);
		std::copy(rgb, rgb+getVideoBufferSize(), m_buffer_video.begin());
		m_new_rgb_frame = true;
	};

	void DepthCallback(void* _depth, uint32_t timestamp)
    {
		Mutex::ScopedLock lock(m_depth_mutex);
		uint16_t* depth = static_cast<uint16_t*>(_depth);
		std::copy(depth, depth+getDepthBufferSize(), m_buffer_depth.begin());
		m_new_depth_frame = true;
	}

	bool getRGB(uint8_t* buffer) 
    {
        if( buffer == NULL )
        {
            fprintf( stderr, "Warning: rgb buffer is null\n" );
            return false;
        }
		Mutex::ScopedLock lock(m_rgb_mutex);
		if (!m_new_rgb_frame)
			return false;
        //memcpy( buffer, &m_buffer_video[0], m_buffer_video.size() * sizeof( uint8_t ) );
        for( int i = 0; i < MED_RES_W * MED_RES_H * 3; i++ )
            buffer[i] = m_buffer_depth[i];
		m_new_rgb_frame = false;
		return true;
	}

	bool getDepth(uint16_t* buffer) 
    {
        if( buffer == NULL )
        {
            fprintf( stderr, "Warning: depth buffer is null\n" );
            return false;
        }
		Mutex::ScopedLock lock(m_depth_mutex);
		if (!m_new_depth_frame)
			return false;
        //memcpy( buffer, &m_buffer_depth[0], m_buffer_depth.size() * sizeof( uint16_t ) );
        for( int i = 0; i < MED_RES_W * MED_RES_H; i++ )
            buffer[i] = m_buffer_depth[i];
		m_new_depth_frame = false;
		return true;
	}
};
