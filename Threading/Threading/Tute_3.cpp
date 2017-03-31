#include "Tute_3.h"

void tute3_ex1()
{
	IntCircularFIFO10 intArray;

	const int MAX_VALUE = 50;
	// publish to queue
	auto publisher = std::thread([&] {
		int count = 0;
		while (count <= MAX_VALUE) {
			if (intArray.Push(count)) {
				std::cout << "++ Pushed - " << count << " to the queue " << std::endl;
				count++;
			}
			// publish wait times
			std::this_thread::sleep_for(std::chrono::seconds(glm::gaussRand(2, 2)));
		}
	});
	// consume from queue
	auto consumer = std::thread([&] {
		int value = 0;
		while (value <= MAX_VALUE) {
			if (intArray.Pop(value)) {
				std::cout << "-- Popped - " << value << " from the queue " << std::endl;
			}
			// consume wait time
			std::this_thread::sleep_for(std::chrono::seconds(glm::gaussRand(1, 1)));
		}
	});
	// threads must be finished before application finishes, results in ABORT error if not joined
	publisher.join();
	consumer.join();
}

void tute3_ex2()
{
	CircularFIFO<float, 20> intArray;
	int push = 1;
	int pull = 2;

	const int MAX_VALUE = 50;
	// publish to queue
	auto publisher = std::thread([&] {
		int count = 0;
		while (count <= MAX_VALUE) {
			if (intArray.Push((float)count)) {
				std::cout << "++ Pushed - " << count << " to the queue " << std::endl;
				count++;
			}
			// publish wait times
			std::this_thread::sleep_for(std::chrono::seconds(glm::gaussRand(push, push)));
		}
	});
	// consume from queue
	auto consumer = std::thread([&] {
		float value = 0.0f;
		while (value <= (float)MAX_VALUE) {
			if (intArray.Pop(value)) {
				std::cout << "-- Popped - " << value << " from the queue " << std::endl;
			}
			else{
				if (value == MAX_VALUE) {
					std::cout << "!! Processing Complete !! " << std::endl;
					break;
				}
			}
			// consume wait time
			std::this_thread::sleep_for(std::chrono::seconds(glm::gaussRand(pull, pull)));
		}
	});
	// threads must be finished before application finishes, results in ABORT error if not joined
	publisher.join();
	consumer.join();
}
