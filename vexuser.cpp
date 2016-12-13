#include <ch.h>     // needs for all ChibiOS programs
#include <hal.h>    // hardware abstraction layer header
#include <vex.h>    // vex library header
#include "config.h" // motor and digital configs

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
	vexDigitalConfigure(dConfig, DIG_CONFIG_SIZE(dConfig));
	vexMotorConfigure(mConfig, MOT_CONFIG_SIZE(mConfig));
}

static int autonToUse = 2;
static char *autons[3] = {
	"basic        (3)",
	"fence push (3-5)",
	"bag drop   (4-6)"
};

void vexUserInit(void)
{
	// if in disabled autonomous, prompt for a selection
	if (vexSpiGetControl() & (kFlagCompetitionSwitch | kFlagAutonomousMode)) {
		int timeout = 100, sel = 0;
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
	vexMotorSet(mClaw, 127);
	vexSleep(200);
	vexMotorSet(mClaw, -127);
	vexSleep(500);
	vexMotorSet(mClaw, 0);
	vexSleep(500);

	if (autonToUse == 0) {
	
	// pushy push push
	vexMotorSet(mDriveLeft, 127);    // drive forward
	vexMotorSet(mDriveRight, -127);
	vexSleep(2500);
	vexMotorSet(mDriveLeft, -127);   // back up
	vexMotorSet(mDriveRight, 127);
	vexSleep(1500);
	vexMotorStopAll();

	} else if( autonToUse == 1 ) {

	vexMotorSet(mLiftLowLeft,   60);  // lift up
	vexMotorSet(mLiftHighLeft,  60);
	vexMotorSet(mLiftLowRight,  60);
	vexMotorSet(mLiftHighRight, 60);
	vexSleep(500);
	vexMotorSet(mLiftLowLeft,   10);  // lift steady
	vexMotorSet(mLiftHighLeft,  10);
	vexMotorSet(mLiftLowRight,  10);
	vexMotorSet(mLiftHighRight, 10);

	vexMotorSet(mDriveLeft, 127);     // forward
	vexMotorSet(mDriveRight, -127);
	vexSleep(2800);
	vexMotorSet(mDriveLeft, -40);     // back up
	vexMotorSet(mDriveRight, 40);
	vexSleep(2300);
	vexMotorSet(mDriveLeft, -80);     // turn
	vexMotorSet(mDriveRight, -80);
	vexMotorSet(mLiftLowLeft,   -60); // drop lift 
	vexMotorSet(mLiftHighLeft,  -60);
	vexMotorSet(mLiftLowRight,  -60);
	vexMotorSet(mLiftHighRight, -60);
	vexSleep(500);
	vexMotorStopAll();

	} else if( autonToUse == 2 ) {

	vexMotorSet(mLiftLowLeft,   -40); // lift down
	vexMotorSet(mLiftHighLeft,  -40);
	vexMotorSet(mLiftLowRight,  -40);
	vexMotorSet(mLiftHighRight, -40);
	vexMotorSet(mDriveLeft, 127);     // straight
	vexMotorSet(mDriveRight, -127);
	vexSleep(2000);
	vexMotorSet(mDriveLeft, 0);       // stop
	vexMotorSet(mDriveRight, 0);
	vexMotorSet(mClaw, 127);          // grab
	vexSleep(500);
	vexMotorSet(mClaw, 50);           // steady
	vexMotorSet(mLiftLowLeft,   127); // lift
	vexMotorSet(mLiftHighLeft,  127);
	vexMotorSet(mLiftLowRight,  127);
	vexMotorSet(mLiftHighRight, 127);
	vexSleep(850);
	vexMotorSet(mLiftLowLeft,   10);  // steady
	vexMotorSet(mLiftHighLeft,  10);
	vexMotorSet(mLiftLowRight,  10);
	vexMotorSet(mLiftHighRight, 10);
	vexMotorSet(mDriveLeft, 80);      // turn
	vexMotorSet(mDriveRight, 80);
	vexSleep(500);
	vexMotorSet(mDriveLeft, 127);     // forward
	vexMotorSet(mDriveRight, -127);
	vexSleep(1500);
	vexMotorSet(mDriveLeft, 0);       // stop
	vexMotorSet(mDriveRight, 0);
	vexMotorSet(mClaw, -127);         // drop
	vexSleep(1000);
	vexMotorSet(mDriveLeft, -40);     // back
	vexMotorSet(mDriveRight, 40);
	vexSleep(1000);
	vexMotorStopAll();

	}

	while (1)
		vexSleep(25);

    return (msg_t)0;
}

static char waVexLcdTask[512];
msg_t vexLcdTask(void *arg)
{
	(void)arg;

	vexTaskRegister("lcdtask");

	while (!chThdShouldTerminate()) {
		vexLcdPrintf(0, 0, "%3.0f%% / %3.0f%%",
			(float)(vexSpiGetMainBattery() - 5000) / 32.0f, (float)(vexSpiGetBackupBattery() - 6000) / 40.0f);

		vexSleep(500);
	}

	return (msg_t)0;
}

constexpr const int liftMaxSpeed = 60;
static int liftTargetLoc = 0;

static char waVexLiftTask[512];
msg_t vexLiftTask(void *arg)
{
	(void)arg;

	vexTaskRegister("lcdtask");

	static int timeout = -1;
	while (!chThdShouldTerminate()) {
		int actual = vexEncoderGet(kVexQuadEncoder_1);
		int diff = liftTargetLoc - actual;
		int speed;
		if (diff != 0) {
			speed = 2 * diff;

			if (timeout == -1)
				timeout = 10;
			else if (timeout > 0)
				timeout--;
			else
				diff++;
		} else {
			timeout = -1, speed = 0;
		}

		vexLcdPrintf(0, 1, "%4d | %4d", actual, speed);
		vexMotorSet(mLiftLowLeft,   speed);
		vexMotorSet(mLiftHighLeft,  speed);
		vexMotorSet(mLiftLowRight,  speed);
		vexMotorSet(mLiftHighRight, speed);

		vexSleep(50);
	}

	return (msg_t)0;
}

msg_t vexOperator(void *arg)
{
	(void)arg;

	// Must call this
	vexTaskRegister("operator");

	chThdCreateStatic(waVexLcdTask, 512, NORMALPRIO - 1, vexLcdTask, nullptr);
	chThdCreateStatic(waVexLiftTask, 512, NORMALPRIO - 1, vexLiftTask, nullptr);

	// Run until asked to terminate
	while (!chThdShouldTerminate()) {
		if (vexControllerGet(Btn7R))
			reset();

		int dy = vexControllerGet(Ch3);
		int dx = -vexControllerGet(Ch4);

		vexMotorSet(mDriveLeft, dy + dx);
		vexMotorSet(mDriveRight, -dy + dx);

		if (vexControllerGet(Btn5U))
			liftTargetLoc += 5;
		else if (vexControllerGet(Btn5D))
			liftTargetLoc -= 5;

		int claw = (vexControllerGet(Btn6U) ? 127 : (vexControllerGet(Btn6D) ? -127 : 0));
		vexMotorSet(mClaw, claw);

		// Don't hog cpu
		vexSleep(100);
	}

	return (msg_t)0;
}



