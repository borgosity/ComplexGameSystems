#include <iostream>
#include "FuzzyApp.h"

int main()
{
	std::cout << "ADM_Tester..." << std::endl;

	FuzzyApp * app = new FuzzyApp();
	
	if (app != nullptr) {
		app->run("AIE", 1280, 720, false);
	}

	delete app;

	std::cout << "...Finished!" << std::endl;
	return 0;
}