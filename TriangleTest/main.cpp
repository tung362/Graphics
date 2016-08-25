//#include "crenderutils.h"
#include "Window.h"

int main()
{
	Window window;
	window.Init();
	while (window.Step())
	{

	}
	window.Term();
	return 0;
}