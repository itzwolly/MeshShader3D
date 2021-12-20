#include "FPSBenchmarkTool.h"

FPSBenchmarkTool::FPSBenchmarkTool() : _debug(true) { }

FPSBenchmarkTool::~FPSBenchmarkTool() { }

void FPSBenchmarkTool::update(float pFps, int pCount) {
	if (pCount <= 0) {
		if (_debug) {
			std::cout << "[FPSBenchmarkTool] Invalid value given for 'pCount' parameter. Expected value should be higher than 0.";
		}
		return;
	}

	if (pCount > 0 && getCount() >= pCount) {
		stop();
	} else {
		if (_averageFps == 0) { // For now we only need 1 benchmark per game load
			addEntry(pFps);
		}
	}
}

void FPSBenchmarkTool::updateManually(float pFps) {
	addEntry(pFps);
}

int FPSBenchmarkTool::stop() {
	if (_fpsList.size() == 0) {
		return 0 ;
	}
	_averageFps = calculateAverageFps(); // store calculated average fps
	
	int count = getCount(); // store the amount of fps entries in the list before deleting

	if (_debug) {
		std::cout << "[FPSBenchmarkTool] Average FPS is: " << _averageFps << " (" << 1000.0f / _averageFps << " ms per frame) average taken over: " << count << " entries" << std::endl;
		std::cout << "[FPSBenchmarkTool] Benchmark taken with following data: \n";

		std::string s;
		for (size_t i = 0; i < _fpsList.size(); i++) {
			s += "\t" + std::to_string(_fpsList[i]) + ((i == _fpsList.size() - 1) ? "\n\n" : ", ") + (((i + 1) % 3 == 0) ? "\n" : "");
		}
		std::cout << s;
	}

	_fpsList.clear(); // clear list of current measures
	return count; // return the amount of fps entries used to run the previous test
}

float FPSBenchmarkTool::getAverageFps() {
	return _averageFps;
}

int FPSBenchmarkTool::getCount() {
	return _fpsList.size();
}

void FPSBenchmarkTool::addEntry(float pFps) {
	_fpsList.push_back(pFps);
}

float FPSBenchmarkTool::calculateAverageFps() {
	size_t size = _fpsList.size();
	float average = 0.0f;
	if (size != 0) {
		average = std::accumulate(_fpsList.begin(), _fpsList.end(), 0.0) / size;
	}
	return average;
}