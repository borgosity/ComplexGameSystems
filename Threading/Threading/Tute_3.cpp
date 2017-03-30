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
				std::cout << "Pushed" << count << " to the queue " << std::endl;
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
				std::cout << "Popped" << value << " from the queue " << std::endl;
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
	CircularFIFO<int, 11> intArray;

	const int MAX_VALUE = 50;
	// publish to queue
	auto publisher = std::thread([&] {
		int count = 0;
		while (count <= MAX_VALUE) {
			if (intArray.Push(count)) {
				std::cout << "Pushed" << count << " to the queue " << std::endl;
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
				std::cout << "Popped" << value << " from the queue " << std::endl;
			}
			// consume wait time
			std::this_thread::sleep_for(std::chrono::seconds(glm::gaussRand(1, 1)));
		}
	});
	// threads must be finished before application finishes, results in ABORT error if not joined
	publisher.join();
	consumer.join();
}
