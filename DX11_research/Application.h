#pragma once
#include "Window.h"
//#include "Timer.h"

class App
{
public:
	App();
	int start(); //Go -> Start
private:
	void do_frame();
private:
	Window wnd_;
	//Timer timer_;
};