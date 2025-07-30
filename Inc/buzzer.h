#include "main.h"
#include "extern.h"
#include "button.h"
#include <stdbool.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

enum notes {
  C4 = 262, 	// 도 261.63Hz
  D4 = 294, 	// 래 293.66Hz
  E4 = 330, 	// 미 329.63Hz
  F4 = 349, 	// 파 349.23Hz
  G4 = 392,	 	// 솔 392.00Hz
  A4 = 440,	 	// 라 440.00Hz
  Bb4 = 467, 	// Bb
  B4 = 494,		// 시 493.88Hz
  C5 = 523,  	// 도 523.25Hz
  D5 = 588,
  E5 = 660,
  F5 = 698,
  Gb5 = 741,
  G5 = 784,
  A5 = 880,
  B5 = 988,
  C6 = 1046,
  D6 = 1176,
  E6 = 1320
};

void noTone();
void buzzer_elevator(void);
void buzzer_floor_select(void);
void check_and_run_buzzer(void);
