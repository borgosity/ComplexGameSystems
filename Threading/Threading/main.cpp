// standard library includes
#include <iostream>
#include <thread>

// source includes
#include "threadFunctions.h"

int main()
{
	std::vector<std::thread> threads;
	std::vector<std::thread> lamdaThreads;
	std::mutex myMutex;
	std::vector<std::thread> vectorThreads;
	// normalizing
	const int vecSize = 50000;
	glm::vec4 myVectors[vecSize] = {};

	// set parent threads to call child thread functions
	//std::thread parent(threadFiller, std::ref(threads));
	//std::thread lamdaParent(threadLamda, std::ref(lamdaThreads), std::ref(myMutex));
	std::thread normalParent(normalizeVectors, myVectors, vecSize, std::ref(vectorThreads));
	// join parent threads
	//parent.join();
	//lamdaParent.join();
	normalParent.join();
	// join child threads
	//joinThreads(threads);
	//joinThreads(lamdaThreads);
	joinThreads(vectorThreads);

	return 0;
}