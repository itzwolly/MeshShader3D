#ifndef FPSBENCHMARKTOOL_HPP
#define FPSBENCHMARKTOOL_HPP

#include <vector>
#include <numeric>
#include <iostream>
#include <string>

class FPSBenchmarkTool {
	public:
		// Constructor
		FPSBenchmarkTool();
		// Destructor
		virtual ~FPSBenchmarkTool();

		// Adds an entry into the fps list as long as the size of the list is less than the specified count.
		void update(float pFps, int pCount = 10);
		// Adds an entry into the fps list to be used later.
		void updateManually(float pFps);
		// Calculates the weighted average and clears the fps list.
		int stop();
		// Returns the average based on the entries in the fps list.
		float getAverageFps();
		// Returns a number based on the amount of entries in the fps list.
		int getCount();
	
	private:
		bool _debug;

		float _averageFps = 0;
		std::vector<float> _fpsList;

		void addEntry(float pFps);
		float calculateAverageFps();
};

#endif
