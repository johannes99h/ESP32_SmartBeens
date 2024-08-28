#line 1 "C:\\Github\\ESP32_SmartBeens_v0\\energy_optimization.hpp"
#if !defined INCLUDED__ESP32_SMART_BEENS_V0__ENERGY_OPTIMIZATION_HPP
#define INCLUDED__ESP32_SMART_BEENS_V0__ENERGY_OPTIMIZATION_HPP


extern unsigned long long time_since_start;
extern unsigned long long boot_count;


int wake_up_from_deep_sleep();
int print_wakeup_reason();
int prepare_deep_sleep();


#endif /* INCLUDED__ESP32_SMART_BEENS_V0__ENERGY_OPTIMIZATION_HPP */