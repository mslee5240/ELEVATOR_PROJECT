#include "main.h"
#include "extern.h"
#include "stepmotor.h"

void dotmatrix_main(uint8_t pattern[]);
void floor_display();
void set_scroll_data(uint8_t** dis_data, int size);
void dotmatrix_print(void);
void dotmatrix_down_scroll (void);
void dotmatrix_up_scroll(void);
void dotmatrix_elevator(void);
