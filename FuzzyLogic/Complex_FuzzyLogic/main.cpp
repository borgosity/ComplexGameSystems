#include "Complex_FuzzyLogicApp.h"

int main() {
	
	auto app = new Complex_FuzzyLogicApp();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}