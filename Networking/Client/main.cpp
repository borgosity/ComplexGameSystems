#include "Client.h"

int main() {
	
	auto app = new Client();
	app->run("AIE", 640, 360, false);
	delete app;

	return 0;
}