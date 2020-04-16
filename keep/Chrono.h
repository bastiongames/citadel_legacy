#ifndef __CHRONO_H__
#define __CHRONO_H__

#include <chrono>
#include "Types.h"

#include <windows.h>

namespace Citadel::Keep {
	struct clock {
	public:
		static u64 now() {
			return std::chrono::high_resolution_clock::now().time_since_epoch().count();
		}
	};

	class StepTimer {
	public:
		StepTimer() {
			lastTime = std::chrono::high_resolution_clock::now();
		}

		void Tick() {
			lastTime = std::chrono::high_resolution_clock::now();
		}

		double ElapsedSeconds() {
			return ElapsedMilliseconds() / 1000.0;
		}

		double ElapsedMilliseconds() {
			return ElapsedMicroseconds() / 100.0;
		}

		double ElapsedMicroseconds() {
			auto currentTime = std::chrono::high_resolution_clock::now();
			return (double)std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count();
		}
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
	};

    class Timer {
    public:
        Timer() {

        }

        double GetTotalSeconds() const {
            if (isStopped) {
                return (pausedTime - startTime - totalIdleTime) * 1.0;
            }
            return (currentTime - startTime - totalIdleTime) * 1.0;
        }

        double GetDeltaSeconds() const {
            return deltaTime;
        }

        void Start() {
            if (isStopped) {
                long long int now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
                totalIdleTime += (now - pausedTime);
                previousTime = now;

                pausedTime = 0;
                isStopped = false;
            }
        }

        void Reset() {
            long long int now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            startTime = now;
            previousTime = now;
            pausedTime = 0;
            isStopped = false;
        }

        void Tick() {
            currentTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            deltaTime = (currentTime - previousTime) * 1.0;
            if (deltaTime < 0.0) {
                deltaTime = 0.0;
            }
        }

        void Stop() {
            if (!isStopped) {
                long long int now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
                pausedTime = now;
                isStopped = true;
            }
        }

    private:
        long long int startTime;
        long long int totalIdleTime;
        long long int pausedTime;
        long long int currentTime;
        long long int previousTime;

        double deltaTime;
        bool isStopped;
    };
}

#endif