#include "SingleThreadedApplication.h"
#include "MultiThreadedApplication.h"

int main()
{
    SingleThreadedApplication app;
	MultiThreadedApplication multiApp;

    while (true)
    {
        //app.Tick();
		multiApp.Tick();
    }

    return 0;
}


