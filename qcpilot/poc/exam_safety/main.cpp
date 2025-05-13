#include <cmath>
#include <cstdint>
#include <cstdio>
#define GET_BYTE(msg, b) ((msg)->data[(b)])
#define MIN(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    (_a < _b) ? _a : _b;                                                       \
  })
#define MAX(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    (_a > _b) ? _a : _b;                                                       \
  })

struct sample_t {
  int value[6];
  int max;
  int min;
};

typedef enum {
  TorqueMotorLimited,  // torque steering command, limited by EPS output torque
  TorqueDriverLimited, // torque steering command, limited by driver's input
                       // torque
} SteeringControlType;

typedef struct {
  // torque cmd limits
  const int max_steer;
  const int max_rate_up;
  const int max_rate_down;
  const int max_rt_delta;
  const uint32_t max_rt_interval;

  const SteeringControlType type;

  // driver torque limits
  const int driver_torque_allowance;
  const int driver_torque_multiplier;

  // motor torque limits
  const int max_torque_error;

  // safety around steer req bit
  const int min_valid_request_frames;
  const int max_invalid_request_frames;
  const uint32_t min_valid_request_rt_interval;
  const bool has_steer_req_tolerance;
} TorqueSteeringLimits;

static bool max_limit_check(int val, const int MAX_VAL, const int MIN_VAL) {
  return (val > MAX_VAL) || (val < MIN_VAL);
}

static bool driver_limit_check(int val, int val_last,
                               const struct sample_t *val_driver,
                               const int MAX_VAL, const int MAX_RATE_UP,
                               const int MAX_RATE_DOWN, const int MAX_ALLOWANCE,
                               const int DRIVER_FACTOR) {

  // torque delta/rate limits
  int highest_allowed_rl = MAX(val_last, 0) + MAX_RATE_UP;
  int lowest_allowed_rl = MIN(val_last, 0) - MAX_RATE_UP;

  // driver
  int driver_max_limit =
      MAX_VAL + (MAX_ALLOWANCE + val_driver->max) * DRIVER_FACTOR;
  int driver_min_limit =
      -MAX_VAL + (-MAX_ALLOWANCE + val_driver->min) * DRIVER_FACTOR;

  // if we've exceeded the applied torque, we must start moving toward 0
  int highest_allowed = MIN(highest_allowed_rl, MAX(val_last - MAX_RATE_DOWN,
                                                    MAX(driver_max_limit, 0)));
  int lowest_allowed = MAX(lowest_allowed_rl, MIN(val_last + MAX_RATE_DOWN,
                                                  MIN(driver_min_limit, 0)));
  std::printf("driver_max_limit=%d, driver_min_limit=%d, highest_allowed=%d, "
              "lowest_allowed=%d\r\n",
              driver_max_limit, driver_min_limit, highest_allowed,
              lowest_allowed);
  std::printf("highest_allowed=%d, lowest_allowed=%d\r\n", highest_allowed,
              lowest_allowed);
  // check for violation
  return max_limit_check(val, highest_allowed, lowest_allowed);
}

static bool rt_rate_limit_check(int val, int val_last, const int MAX_RT_DELTA) {

  // *** torque real time rate limit check ***
  int highest_val = MAX(val_last, 0) + MAX_RT_DELTA;
  int lowest_val = MIN(val_last, 0) - MAX_RT_DELTA;

  // check for violation
  return max_limit_check(val, highest_val, lowest_val);
}

int main(void) {
  sample_t sample{{-25, -25, -25, -25, -25, -25}, -25, -25};
  const int desired_torque = -800;
  const int desired_torque_last = -800;
  TorqueSteeringLimits limits = {
      .max_steer = 800,
      .max_rate_up = 10,
      .max_rate_down = 25,
      .max_rt_delta = 300,
      .max_rt_interval = 250000,
      .type = TorqueDriverLimited,
      .driver_torque_allowance = 15,
      .driver_torque_multiplier = 1,

  };

  bool violation = driver_limit_check(
      desired_torque, desired_torque_last, &sample, limits.max_steer,
      limits.max_rate_up, limits.max_rate_down, limits.driver_torque_allowance,
      limits.driver_torque_multiplier);
  std::printf("%d\r\n", violation);

  int rt_torque_last = -800;
  violation |=
      rt_rate_limit_check(desired_torque, rt_torque_last, limits.max_rt_delta);
  std::printf("%d\r\n", violation);

  return 0;
}