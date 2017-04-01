#include "Tute_1.h"

void exercise_1()
{
	std::vector<std::thread> threads;
	// set parent threads to call child thread functions
	std::thread parent(threadFiller, std::ref(threads));
	// join parent threads
	parent.join();
	// join child threads
	joinThreads(threads);
}

void exercise_2()
{
	std::vector<std::thread> lamdaThreads;
	std::mutex myMutex;
	// set parent threads to call child thread functions
	std::thread lamdaParent(threadLamda, std::ref(lamdaThreads), std::ref(myMutex));
	// join parent threads
	lamdaParent.join();
	// join child threads
	joinThreads(lamdaThreads);
}

void exercise_3()
{
	std::vector<std::thread> vectorThreads;
	// normalizing
	const int vecSize = 500;
	glm::vec4 myVectors[vecSize] = {};
	initVectors(myVectors, vecSize);

	// set parent threads to call child thread functions
	//std::thread normalParent(normalizeVectorsLambda, myVectors, vecSize, std::ref(vectorThreads));
	// join parent threads
	//normalParent.join();
	// join child threads

	int chunk = vecSize / 10;

	for (int i = 0; i < 10; i++)
	{
		vectorThreads.push_back(std::thread(normalizeVectors, myVectors, vecSize, i*chunk, (i + 1) * chunk));
		std::cout << "Batch PUSH - " << i << std::endl;
	}
	//std::cout << "Total - " << total << std::endl;

	joinThreads(vectorThreads);
}
