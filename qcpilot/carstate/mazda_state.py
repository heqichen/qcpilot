from opendbc.car.structs import QcMazdaStateT, QcMazdaState
from opendbc.can.parser import CANParser

qcMazdaState: QcMazdaStateT = QcMazdaState()
stateUpdated: bool = False


def reset_mazda_state():
    global qcMazdaState
    global stateUpdated
    qcMazdaState = QcMazdaState()
    stateUpdated = False


def update_mazda_state(cp: CANParser):
    global qcMazdaState
    global stateUpdated
    stateUpdated = True

    qcMazdaState.engineRpm = int(cp.vl["ENGINE_DATA"]["RPM"])
    qcMazdaState.engineSpeedKph = cp.vl["ENGINE_DATA"]["SPEED"]
    qcMazdaState.wheelSpeedFLKph = cp.vl["WHEEL_SPEEDS"]["FL"]
    qcMazdaState.wheelSpeedFRKph = cp.vl["WHEEL_SPEEDS"]["FR"]
    qcMazdaState.wheelSpeedRLKph = cp.vl["WHEEL_SPEEDS"]["RL"]
    qcMazdaState.wheelSpeedRRKph = cp.vl["WHEEL_SPEEDS"]["RR"]

    qcMazdaState.cruiseButtonState = cp.vl["CRZ_BTNS"]["MRCC"] == 1
    qcMazdaState.lkasButtonState = cp.vl["CRZ_BTNS"]["LKAS"] == 1
    qcMazdaState.accelX = cp.vl["BRAKE"]["VEHICLE_ACC_X"]
    qcMazdaState.accelY = cp.vl["BRAKE"]["VEHICLE_ACC_Y"]

    # It should be the logic value of brake pedal, map [512, 1024] to [0.0, 1.0]
    qcMazdaState.brakePressure = float(cp.vl["BRAKE"]["BRAKE_PRESSURE"])
    qcMazdaState.throttlePressure = 0.0

    # CRZ = KD + KL
    # KD = ACC
    # True if ACC is ready, but not work. speed is three dots
    qcMazdaState.isCruiseAvailable = cp.vl["CRZ_CTRL"]["CRZ_AVAILABLE"] == 1
    qcMazdaState.isCruiseActive = cp.vl["CRZ_CTRL"]["CRZ_ACTIVE"] == 1
    qcMazdaState.isAccActive = cp.vl["CRZ_CTRL"]["ACC_ACTIVE"] == 1

    qcMazdaState.isLkasBlocked = cp.vl["STEER_RATE"]["LKAS_BLOCK"] == 1

    # print(f"publish message: {qcMazdaState.isCruiseAvailable}")


def get_mazda_state():
    global qcMazdaState
    global stateUpdated

    return qcMazdaState if stateUpdated else None
