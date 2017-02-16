#include "lift.hpp"

#include <ch.h>
#include <hal.h>
#include <vex.h>
#include "config.h"

#include <atomic>

static std::atomic<int> liftTargetLoc;

static char waVexLiftTask[512];
msg_t vexLiftTask(void *);

namespace lift {
	void init(void) {
		static bool running = false;
		if (!running) {
			liftTargetLoc.store(0);
			chThdCreateStatic(waVexLiftTask, 512, NORMALPRIO - 1, vexLiftTask, nullptr);
			running = true;
		}
	}

	void set(const int& target) {
		auto t = target;
		if (t < -10)
			t = -10;
		else if (t > liftMaxHeight)
			t = liftMaxHeight;

		liftTargetLoc.store(t);
	}

	void raise(void) {
		liftTargetLoc.store(liftTargetLoc.load() + 5);
	}

	void lower(void) {
		liftTargetLoc.store(liftTargetLoc.load() - 5);
	}
}

msg_t vexLiftTask(void *arg)
{
	(void)arg;

	vexTaskRegister("lift");

	//static int timeout = -1;
	while (!chThdShouldTerminate()) {
		int actual = vexEncoderGet(kVexQuadEncoder_1);
		int diff = liftTargetLoc.load() - actual;
		int speed = 0; 
		if (diff != 0) {
			/*if (timeout == -1)
				timeout = 10;
			else if (timeout > 0)
				timeout--;
			else
				diff *= 2;*/

			speed = 2 * diff;
		} else {
			/*timeout = -1,*/ speed = 0;
		}

		vexLcdPrintf(0, 1, "A%02d D%d S%d", actual, diff, speed);
		vexMotorSet(mLiftLowLeft,   speed);
		vexMotorSet(mLiftHighLeft,  speed);
		vexMotorSet(mLiftLowRight,  speed);
		vexMotorSet(mLiftHighRight, speed);

		vexSleep(50);
	}

	return static_cast<msg_t>(0);
}

