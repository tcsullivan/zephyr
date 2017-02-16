/**
 * @file config.h
 * @brief Defines motor and digital port configurations.
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/**
 * Creates a default entry in the digital port configuration.
 * @param pin the pin to configure
 * @return the entry for the configuration
 */
constexpr auto digiDefault(const auto& pin)
{
	return vexDigiCfg { pin, kVexSensorDigitalInput, kVexConfigInput, 0 };
}

/**
 * Creates a default entry in the motor port configuration.
 * @param port the port to configure
 * @param type the type of the motor
 * @return the entry for the configuration
 */
constexpr auto motorDefault(const auto& port, const tVexMotorType& type = kVexMotorUndefined)
{
	return vexMotorCfg { port, type, kVexMotorNormal, kVexSensorNone, 0 };
}

/** The left drive motor's port. */
constexpr auto mDriveLeft     = kVexMotor_5;

/** The right drive motor's port. */
constexpr auto mDriveRight    = kVexMotor_6;

/** The lower right lift motor's port. */
constexpr auto mLiftLowRight  = kVexMotor_7;

/** The upper right lift motor's port. */
constexpr auto mLiftHighRight = kVexMotor_8;

/** The lower left lift motor's port. */
constexpr auto mLiftLowLeft   = kVexMotor_3;

/** The upper left lift motor's port. */
constexpr auto mLiftHighLeft  = kVexMotor_4;

/** The claw motor's port. */
constexpr auto mClaw          = kVexMotor_1;

/** The IME channel for the lower left lift motor. */
constexpr auto iLiftLowLeft  = kImeChannel_1;
/** The IME channel for the lower right lift motor. */
constexpr auto iLiftLowRight = kImeChannel_2;

/**
 * Defines the motor port configuration for the cortex.
 */
constexpr vexMotorCfg mConfig[kVexMotorNum] = {
        motorDefault( mDriveLeft ),
        motorDefault( mDriveRight ),
        motorDefault( mClaw ),
        motorDefault( mLiftHighLeft,  kVexMotor393S ),
        motorDefault( mLiftHighRight, kVexMotor393S ),
        { mLiftLowLeft,  kVexMotor393S, kVexMotorNormal, kVexSensorIME, iLiftLowLeft },
        { mLiftLowRight, kVexMotor393S, kVexMotorNormal, kVexSensorIME, iLiftLowRight },
};

/**
 * Defines the digital port configuration for the cortex.
 */
constexpr vexDigiCfg dConfig[kVexDigital_Num] = {
        { kVexDigital_1,    kVexSensorQuadEncoder,   kVexConfigQuadEnc1,    kVexQuadEncoder_1 },
        { kVexDigital_2,    kVexSensorQuadEncoder,   kVexConfigQuadEnc2,    kVexQuadEncoder_1 },
        digiDefault(kVexDigital_3),
        digiDefault(kVexDigital_4),
        digiDefault(kVexDigital_5),
        digiDefault(kVexDigital_6),
        digiDefault(kVexDigital_7),
        digiDefault(kVexDigital_8),
        digiDefault(kVexDigital_9),
        digiDefault(kVexDigital_10),
        digiDefault(kVexDigital_11),
        digiDefault(kVexDigital_12),
};

#endif // CONFIG_H_
