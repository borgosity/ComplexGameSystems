#pragma once
#include <iostream>
#include <vector>
#include <mutex>
// opengl includes
#include "glm\glm.hpp"

void print();
void printChar(char i);
void printInt(int i);

void threadFiller(std::vector<std::thread> &threads);
void threadLamda(std::vector<std::thread> &threads, std::mutex &myMutex);
void joinThreads(std::vector<std::thread> &threads);

//void normalizeVectors(glm::vec4 * a_vectors, int a_size, std::vector<std::thread> &threads);
void normalizeVectors(glm::vec4 * a_vectors, int a_size, int low, int high);
