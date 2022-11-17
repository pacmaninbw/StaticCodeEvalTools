#ifndef CC_UTITLTY_TIMER_H
#define CC_UTITLTY_TIMER_H

/*
 * Chernick Consulting Utility timer class.
 * Encapsulates execution timing for all or parts of a program.
 */

#include <chrono>
#include <iostream>
#include <string>

class UtilityTimer
{
public:
	UtilityTimer() = default;
	~UtilityTimer() = default;
	void startTimer() noexcept
	{
		start = std::chrono::system_clock::now();
	}
	void stopTimerAndReport(std::string whatIsBeingTimed) noexcept
	{
		end = std::chrono::system_clock::now();

		std::chrono::duration<double> elapsed_seconds = end - start;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		double ElapsedTimeForOutPut = elapsed_seconds.count();

		std::cout << "finished " << whatIsBeingTimed << std::ctime(&end_time)
			<< "elapsed time in seconds: " << ElapsedTimeForOutPut << "\n" << "\n" << "\n";
	}

private:
	std::chrono::time_point<std::chrono::system_clock> start{}, end{};
};

#endif // CC_UTITLTY_TIMER_H
