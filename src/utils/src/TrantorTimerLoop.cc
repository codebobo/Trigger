#include "TrantorTimerLoop.h"

std::once_flag TrantorTimerLoop::once_;
TrantorTimerLoop* TrantorTimerLoop::timer_loop_ptr_ = NULL;

