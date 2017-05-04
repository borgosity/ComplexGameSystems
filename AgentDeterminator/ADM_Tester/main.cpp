#include <iostream>
#include "FuzzyApp.h"

int main()
{
	std::cout << "ADM_Tester..." << std::endl;

	FuzzyApp * app = new FuzzyApp();
	
	if (app != nullptr) {
		app->run("AIE", SCREEN_W, SCREEN_H, false);
	}

	delete app;

	std::cout << "...Finished!" << std::endl;
	return 0;
}