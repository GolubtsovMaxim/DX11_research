#include "Application.h"
#include "sstream"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLIne,
	int nCmdShow
)
{

	try
	{
		return App{}.start();
	}
	catch (const BaseException& base_exception)
	{
		MessageBox(nullptr, base_exception.what(), base_exception.get_type(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (std::exception std_exception)
	{
		MessageBox(nullptr, std_exception.what(), "STANDARD EXCEPTIOM", MB_OK | MB_ICONEXCLAMATION);
	}
	catch ( ... )
	{
		MessageBox(nullptr, "NO INFO ON EXCEPTION", "UNKNOWN EXCEPTION", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}