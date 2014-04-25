//=================================================================
//
// GFrameHandler 1.0 (for Clanlib)
//
// (C) Copyright 2006 Wong Chin Foo.
// Distributed under the Boost Software License, Version 1.0.
// ( See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
//=================================================================


#include "GFrameHandler.h"
#include <ClanLib/core.h>


GFrameHandler::GFrameHandler() :
frame_count(0),
last_fps_time(CL_System::get_time()),
previous_time(last_fps_time),
elapsed_time(0),
fps(0),
tpf(0),
tpf_limit(20)
{}


void GFrameHandler::reset(void)
{
	frame_count = 0,
	last_fps_time = CL_System::get_time(),
	previous_time = last_fps_time,
	elapsed_time = 0,
	fps = 0,
	tpf = 0;
    tpf_limit = 20;
}


void GFrameHandler::set_fps_limit(int fps)
{
	if(fps < 1)
		return;

	float sleep_interval = 1000.0f / (float)fps;

	tpf_limit = (int)(sleep_interval+0.495f);
}


void GFrameHandler::set_tpf_limit(int millisecond)
{
	tpf_limit = millisecond;
}


void GFrameHandler::limit_frame(void)
{
	tpf = CL_System::get_time() - previous_time;

    if(tpf < tpf_limit)
        CL_System::sleep(tpf_limit - tpf - 1);

	while(tpf < tpf_limit)
	{
		tpf = CL_System::get_time() - previous_time;
	}
}


void GFrameHandler::limit_frame_fast(void)
{
	tpf = CL_System::get_time() - previous_time;

    if(tpf < tpf_limit)
        CL_System::sleep(tpf_limit - tpf - 1);
}


void GFrameHandler::calculate(void)
{
	unsigned int curr_time = CL_System::get_time();

	frame_count++;
	if(curr_time - last_fps_time >= 1000)
	{
		fps = (int)(  frame_count * ( 2.0f - (float)(curr_time - last_fps_time)*0.001f )
					+ 0.495f  );
		frame_count = 0;
		last_fps_time = curr_time;
	}

	elapsed_time = curr_time - previous_time;
	previous_time = curr_time;
}


int GFrameHandler::get_fps(void) const
{
	return fps;
}


int GFrameHandler::get_elapsed_time(void) const
{
	return elapsed_time;
}
