#include "threadFunctions.h"



void print() {
	static std::mutex myMutex;
	std::lock_guard<std::mutex> guard(myMutex);

	printf("New Mutex thread, \n");
	printf("-> line one, \n");
	printf("--> line two. \n");
}
void printChar(char i) {
	printf("%i\n", i);
	printf("-> %i\n", i);
	printf("--> %i\n", i);
}

void printInt(int i) {
	printf("%i\n", i);
	printf("-> %i\n", i);
	printf("--> %i\n", i);
}

void threadFiller(std::vector<std::thread> &threads) {
	for (size_t i = 0; i < 30; i++)
	{
		threads.push_back(std::thread(print));
		threads.push_back(std::thread(printChar, '+'));
		threads.push_back(std::thread(printInt, 0));
	}
}

void threadLamda(std::vector<std::thread>& threads, std::mutex &myMutex)
{
	for (size_t i = 0; i < 30; i++)
	{
		threads.push_back(std::thread(
			[&myMutex]() {
				std::lock_guard<std::mutex> guard(myMutex);
				printf("New Lamda thread, \n");
				printf("-> Lamda one, \n");
				printf("--> Lamda two. \n");
			}
		));
		threads.push_back(std::thread(
			[](int i) {
				printf("%i\n", i);
				printf("-> %i\n", i);
				printf("--> %i\n", i);
			}, i
		));
	}
}

void joinThreads(std::vector<std::thread>& threads)
{
	for (auto& thread : threads) {
		thread.join();
	}
}

void normalizeVectors(glm::vec4 * a_vectors, int a_size, std::vector<std::thread>& threads)
{
	if (a_size <= 100) {
		for (int i = 0; i < a_size; i++)
		{
			threads.push_back(std::thread(
				[&](int i) {
				a_vectors[i] = glm::normalize(a_vectors[i]);
			}, i
			));
			printf("PUSH - %i\n", i);
		}
	}
	else {
		int chunk = a_size / 10;
		for (int i = 0; i < 10; i++)
		{
			threads.push_back(std::thread(
				[&](int low, int high) {
						for (int j = low; j < high; j++)
						{
							a_vectors[j] = glm::normalize(a_vectors[j]);
							std::cout << "count - " << j << std::endl;
						}
					}, i * chunk, (i+1) * chunk
				));
			printf("Batch PUSH - %i\n", i);
		}
	}

}

void normalizeVectors(glm::vec4 * a_vectors, int a_size, int low, int high)
{
	int total = 0;
	for (int j = low; j < high; j++)
	{
		a_vectors[j] = glm::normalize(a_vectors[j]);
		total++;
		std::cout << total << " - " << j << std::endl;
	}
}
