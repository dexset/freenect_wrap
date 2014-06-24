#pragma once
#include "libfreenect.hpp"
#include "pthread.h"
#include "vector"
#include "stdio.h"

class Mutex 
{
public:
	Mutex() 
    { pthread_mutex_init( &m_mutex, NULL ); }
	void lock() 
    { pthread_mutex_lock( &m_mutex ); }
	void unlock() 
    { pthread_mutex_unlock( &m_mutex ); }

	class ScopedLock
	{
		Mutex & _mutex;
	public:
		ScopedLock(Mutex & mutex)
			: _mutex(mutex)
		{ _mutex.lock(); }
		~ScopedLock()
		{ _mutex.unlock(); }
	};
private:
	pthread_mutex_t m_mutex;
};

class BasicFreenectDevice : public Freenect::FreenectDevice 
{
public:
	BasicFreenectDevice(freenect_context *_ctx, int _index)
		: Freenect::FreenectDevice(_ctx, _index), 
        m_buffer_depth(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes),
        m_buffer_video(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes), 
        m_new_rgb_frame(false), m_new_depth_frame(false) {}

	void VideoCallback(void* _rgb, uint32_t timestamp) 
    {
        fprintf( stderr, "Got rgb\n" );
		Mutex::ScopedLock lock(m_rgb_mutex);
		uint8_t* rgb = static_cast<uint8_t*>(_rgb);
		std::copy(rgb, rgb+getVideoBufferSize(), m_buffer_video.begin());
		m_new_rgb_frame = true;
	};

	void DepthCallback(void* _depth, uint32_t timestamp)
    {
        fprintf( stderr, "Got depth\n" );
		Mutex::ScopedLock lock(m_depth_mutex);
		uint16_t* depth = static_cast<uint16_t*>(_depth);
		std::copy(depth, depth+getDepthBufferSize(), m_buffer_depth.begin());
		m_new_depth_frame = true;
	}

	bool getRGB(std::vector<uint8_t> &buffer) 
    {
		Mutex::ScopedLock lock(m_rgb_mutex);
		if (!m_new_rgb_frame)
			return false;
		buffer.swap(m_buffer_video);
		m_new_rgb_frame = false;
		return true;
	}

	bool getDepth(std::vector<uint16_t> &buffer) 
    {
		Mutex::ScopedLock lock(m_depth_mutex);
		if (!m_new_depth_frame)
			return false;
		buffer.swap(m_buffer_depth);
		m_new_depth_frame = false;
		return true;
	}

private:
	std::vector<uint16_t> m_buffer_depth;
	std::vector<uint8_t> m_buffer_video;
	Mutex m_rgb_mutex;
	Mutex m_depth_mutex;
	bool m_new_rgb_frame;
	bool m_new_depth_frame;
};
