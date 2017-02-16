#include "claw.hpp"

#include <ch.h>
#include <hal.h>
#include <vex.h>

#include "config.h"

#include <atomic>

static std::atomic<int> clawSpeed;
static std::atomic<int> clawTime;

static char waVexClawTask[512];
msg_t vexClawTask(void *);

namespace claw {
	void init(void) {
		static bool running = false;
		if (!running) {
			clawSpeed.store(0);
			clawTime.store(0);
			chThdCreateStatic(waVexClawTask, 512, NORMALPRIO - 1, vexClawTask, nullptr);
			running = true;
		}
	}

	void doClaw(int speed, int time = -1) {
		clawSpeed.store(speed);
		clawTime.store(time);
	}

	void open(void) {
		doClaw(-127, openTime);
	}

	void close(void) {
		doClaw(127, closeTime);
	}

	void hold(void) {
		doClaw(70); // inf. hold
		vexSleep(closeTime); // give it time to close
	}

	void unhook(void) {
		vexMotorSet(mClaw, 127); // can't async this
		vexSleep(500);
		open(); 
	}
}

msg_t vexClawTask(void *arg)
{
	(void)arg;

	vexTaskRegister("claw");

	while (!chThdShouldTerminate()) {
		auto time = clawTime.load();
		if (time != 0) {
			vexMotorSet(mClaw, clawSpeed.load());

			if (time > 0) {
				vexSleep(time);
				clawTime.store(0);
			}
		}

		vexSleep(100);
	}

	return 0;
}
