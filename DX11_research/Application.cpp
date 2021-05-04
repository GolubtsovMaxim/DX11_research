#include "Application.h"

//#include <iomanip>
//#include "sstream"

App::App()
	:
	wnd_(800, 600, "Start_Window")
{}

int App::start()
{
	while (true)
	{
		if (const auto code = Window::process_messages())
		{
			return *code;
		}
		do_frame();
	}
}

void App::do_frame()
{
	/*const float	time = timer_.peek();
	std::ostringstream oss;
	oss << "Time elapsed: " << std::setprecision(1) << std::fixed << time << "s";
	wnd_.set_title(oss.str());
	const float c = 0.5f;
	wnd_.graphics().clear_buffer(200, c, 0.5);
	wnd_.graphics().end_frame();*/
}
