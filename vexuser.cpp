#include <ch.h>     // needs for all ChibiOS programs
#include <hal.h>    // hardware abstraction layer header
#include <vex.h>    // vex library header

#include "config.h" // motor and digital configs
#include "lift.hpp" // lift system
#include "claw.hpp"

// missing stdlib functions
extern "C" {
	void _exit(int code) {
		vexLcdPrintf(0, 0, "exit(%d)!", code);
		vexLcdPrintf(0, 1, "halting...");
		while (1);
	}

	void _kill(int pid) {
		(void)pid; // TODO...?
	}

	int _getpid(void) {
		return 0; // TODO..?
	}
}

void reset(void)
{
	uint32_t aircr = *((uint32_t *)0xE000ED0C);	
	aircr = (aircr & 0xFFFF) | (0x5FA << 16) | 5;
	*((volatile uint32_t *)0xE000ED0C) = aircr;
	asm("DSB");
	while (1);
}

// music for autonomous
static char *getiton = 
	"GetItOn:d=8,0=5,b=125:16d#5,16d5,16c5,16p,2f4.,16d#5,16d5,16c5,16p,2f4.,16d#5,16d5,16c5,16p,2f4.,16d#5,16d5,16a#4,16p,2c5.,1f4";

// called once
void vexUserSetup(void)
{
	vexDigitalConfigure(const_cast<vexDigiCfg*>(dConfig), DIG_CONFIG_SIZE(dConfig));
	vexMotorConfigure(const_cast<vexMotorCfg*>(mConfig), MOT_CONFIG_SIZE(mConfig));
}

static int autonToUse = 2;
static char *autons[3] = {
	"basic        (3)",
	"fence push (3-5)",
	"bag drop   (4-6)"
};

void vexUserInit(void)
{
	lift::init(); // start the lift system
	claw::init(); // start the claw system

	// if in disabled autonomous, prompt for a selection
	if (vexSpiGetControl() & (kFlagCompetitionSwitch | kFlagAutonomousMode)) {
		int timeout = 100, sel = autonToUse;
		while (1) {
			vexLcdPrintf(0, 0, "select auton: %2d", (timeout / 10));
			if (timeout-- <= 0)
				break;

			vexLcdPrintf(0, 1, autons[sel]);

			auto btn = vexLcdButtonGet(0);
			if ((btn & kLcdButtonLeft) && sel > 0)
				sel--;
			else if ((btn & kLcdButtonRight) && sel < 2)
				sel++;
			else if (btn & kLcdButtonCenter) {
				autonToUse = sel;
				break;
			}

			vexSleep(100);
		}

		vexLcdPrintf(0, 0, "auton selected: ");
		vexLcdPrintf(0, 1, autons[autonToUse]);
	}
}

msg_t vexAutonomous(void *arg)
{
    (void)arg;

    vexTaskRegister("auton");

	// jam out
	vexAudioPlayRtttl(getiton, 100, 0);

	// drop the star
	claw::unhook(); // 500ms

	if (autonToUse == 0) {
	
	// pushy push push
	vexMotorSet(mDriveLeft, 127);    // drive forward
	vexMotorSet(mDriveRight, -127);
	vexSleep(2200);
	vexMotorSet(mDriveLeft, -127);   // back up
	vexMotorSet(mDriveRight, 127);
	vexSleep(1200);

	} else if( autonToUse == 1 ) {

	lift::set(60);
	vexMotorSet(mDriveLeft, 127);     // forward
	vexMotorSet(mDriveRight, -127);
	vexSleep(2800);
	vexMotorSet(mDriveLeft, -40);     // back up
	vexMotorSet(mDriveRight, 40);
	vexSleep(2300);
	vexMotorSet(mDriveLeft, -80);     // turn
	vexMotorSet(mDriveRight, -80);
	lift::set(0);
	
	} else if( autonToUse == 2 ) {

	lift::set(-10);
	vexMotorSet(mDriveLeft, 127);     // straight
	vexMotorSet(mDriveRight, -127);
	vexSleep(1000);					  // 2200ms
	vexMotorSet(mDriveLeft, 0);       // stop
	vexMotorSet(mDriveRight, 0);
	claw::hold();					  // 3100ms
	lift::set(90);
	vexMotorSet(mDriveLeft, -80);      // turn
	vexMotorSet(mDriveRight, -80);
	vexSleep(500);					  // 3600ms
	vexMotorSet(mDriveLeft, 127);     // forward
	vexMotorSet(mDriveRight, -127);
	vexSleep(1400);					  // 5000ms
	vexMotorSet(mDriveLeft, 0);       // stop
	vexMotorSet(mDriveRight, 0);
	claw::open();
	vexSleep(500);					  // 5500ms
	vexMotorSet(mDriveLeft, -40);     // back
	vexMotorSet(mDriveRight, 40);
	vexSleep(800);					  // 6300ms

	// DRAFT

	vexMotorSet(mDriveLeft, -80);      // turn
	vexMotorSet(mDriveRight, -80);
	vexSleep(1400);					  // 7700ms
	lift::set(0);
	vexMotorSet(mDriveLeft, 127);     // straight
	vexMotorSet(mDriveRight, -127);
	vexSleep(800);					  // 8500ms
	vexMotorSet(mDriveLeft, 0);       // stop
	vexMotorSet(mDriveRight, 0);
	lift::set(-10);
	claw::hold();					  // 9400ms
	lift::set(90);
	vexSleep(400);					  // 9800ms
	vexMotorSet(mDriveLeft, -80);     // turn
	vexMotorSet(mDriveRight, -80);
	vexSleep(1000);					  // 10,800ms

	}

	vexMotorStopAll();
	while (1)
		vexSleep(25);

    return (msg_t)0;
}

static char waVexLcdTask[512];
msg_t vexLcdTask(void *arg)
{
	(void)arg;

	vexTaskRegister("lcdt");

	while (!chThdShouldTerminate()) {
		vexLcdPrintf(0, 0, "%3.0f%% / %3.0f%%",
			(float)(vexSpiGetMainBattery() - 5000) / 32.0f, (float)(vexSpiGetBackupBattery() - 6000) / 40.0f);

		vexSleep(500);
	}

	return (msg_t)0;
}

msg_t vexOperator(void *arg)
{
	(void)arg;

	auto joy = reinterpret_cast<jsdata2*>(vexSpiGetJoystickDataPtr(1));

	// Must call this
	vexTaskRegister("operator");

	chThdCreateStatic(waVexLcdTask, 512, NORMALPRIO - 1, vexLcdTask, nullptr);

	// Run until asked to terminate
	while (!chThdShouldTerminate()) {
		if (joy->Btn7R)
			reset();
	
		int dy = vexControllerGet(Ch3);
		int dx = vexControllerGet(Ch4);

		vexMotorSet(mDriveLeft, dy - dx);
		vexMotorSet(mDriveRight, -dy - dx);

		if (joy->Btn5U)
			lift::raise();
		else if (joy->Btn5D)
			lift::lower();

		if (joy->Btn8D)
			lift::set(0);
		else if (joy->Btn8L)
			lift::set(30);
		else if (joy->Btn8R)
			lift::set(60);
		else if (joy->Btn8U)
			lift::set(90);

		int claw = (joy->Btn6U ? 127 : (joy->Btn6D ? -127 : 0));
		vexMotorSet(mClaw, claw);

		// Don't hog cpu
		vexSleep(100);
	}

	return (msg_t)0;
}



