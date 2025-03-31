#define DEBUG_MODULE "ESTIMATOR"
#include "debug.h"

#include "cfassert.h"
#include "estimator.h"
#include "estimator_complementary.h"

#ifndef DISABLE_KALMAN_ESTIMATOR
#include "estimator_kalman.h"
#endif

#define DEFAULT_ESTIMATOR complementaryEstimator
static StateEstimatorType currentEstimator = anyEstimator;

static void initEstimator();

typedef struct {
  void (*init)(void);
  bool (*test)(void);
  void (*update)(state_t *state, sensorData_t *sensors, control_t *control, const uint32_t tick);
} EstimatorFcns;

static EstimatorFcns estimatorFunctions[] = {
  {.init = 0, .test = 0, .update = 0}, // Any
  {.init = estimatorComplementaryInit, .test = estimatorComplementaryTest, .update = estimatorComplementary},
  #ifndef DISABLE_KALMAN_ESTIMATOR
  {.init = estimatorKalmanInit, .test = estimatorKalmanTest, .update = estimatorKalman},
  #endif
};


void stateEstimatorInit(StateEstimatorType estimator) {
  if (estimator < 0 || estimator >= StateEstimatorTypeCount) {
    return;
  }

  currentEstimator = estimator;

#ifdef DISABLE_KALMAN_ESTIMATOR
  // Force complementary estimator if Kalman was requested
  if (currentEstimator == StateEstimatorTypeCount - 1) {  // This would have been kalmanEstimator
    currentEstimator = complementaryEstimator;
  }
#endif

  if (anyEstimator == currentEstimator) {
    currentEstimator = DEFAULT_ESTIMATOR;
  }

  StateEstimatorType forcedEstimator = ESTIMATOR_NAME;
  if (forcedEstimator != anyEstimator) {
    DEBUG_PRINT("Estimator type forced\n");
    currentEstimator = forcedEstimator;
  }

#ifdef DISABLE_KALMAN_ESTIMATOR
  // Final safety check
  if (currentEstimator >= StateEstimatorTypeCount) {
    currentEstimator = complementaryEstimator;
  }
#endif

  initEstimator();

  DEBUG_PRINT("Using estimator %d\n", currentEstimator);
}

StateEstimatorType getStateEstimator(void) {
  return currentEstimator;
}

static void initEstimator() {
  estimatorFunctions[currentEstimator].init();
}

bool stateEstimatorTest(void) {
  return estimatorFunctions[currentEstimator].test();
}

void stateEstimator(state_t *state, sensorData_t *sensors, control_t *control, const uint32_t tick) {
  estimatorFunctions[currentEstimator].update(state, sensors, control, tick);
}
