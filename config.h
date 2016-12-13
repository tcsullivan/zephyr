#ifndef CONFIG_H_
#define CONFIG_H_

// Digi IO configuration
static  vexDigiCfg  dConfig[kVexDigital_Num] = {
        { kVexDigital_1,    kVexSensorQuadEncoder,   kVexConfigQuadEnc1,    kVexQuadEncoder_1 },
        { kVexDigital_2,    kVexSensorQuadEncoder,   kVexConfigQuadEnc2,    kVexQuadEncoder_1 },
        { kVexDigital_3,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
        { kVexDigital_4,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
        { kVexDigital_5,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
        { kVexDigital_6,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
        { kVexDigital_7,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
        { kVexDigital_8,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
        { kVexDigital_9,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
        { kVexDigital_10,   kVexSensorDigitalInput,  kVexConfigInput,       0 },
        { kVexDigital_11,   kVexSensorDigitalInput,  kVexConfigInput,       0 },
        { kVexDigital_12,   kVexSensorDigitalInput,  kVexConfigInput,       0 }
};

#define mDriveLeft     kVexMotor_5
#define mDriveRight    kVexMotor_6
#define mLiftLowRight  kVexMotor_7
#define mLiftHighRight kVexMotor_8
#define mLiftLowLeft   kVexMotor_3
#define mLiftHighLeft  kVexMotor_4
#define mClaw          kVexMotor_1

#define iLiftLowLeft   kImeChannel_1
#define iLiftLowRight  kImeChannel_2

static  vexMotorCfg mConfig[kVexMotorNum] = {
        { mDriveLeft,     kVexMotorUndefined, kVexMotorNormal, kVexSensorNone, 0 },
        { mDriveRight,    kVexMotorUndefined, kVexMotorNormal, kVexSensorNone, 0 },
        { mLiftHighLeft,  kVexMotor393S,      kVexMotorNormal, kVexSensorNone, 0 },
        { mLiftHighRight, kVexMotor393S,      kVexMotorNormal, kVexSensorNone, 0 },
        { mLiftLowLeft,   kVexMotor393S,      kVexMotorNormal, kVexSensorIME,  iLiftLowLeft },
        { mLiftLowRight,  kVexMotor393S,      kVexMotorNormal, kVexSensorIME,  iLiftLowRight },
        { mClaw,          kVexMotorUndefined, kVexMotorNormal, kVexSensorNone, 0 },
};

#endif // CONFIG_H_
