#include <linux/config.h>

#define  CONFIG_CC_OPTIMIZE_FOR_SIZE 0
#if defined (CONFIG_RALINK_MT7620) || defined (CONFIG_RALINK_MT7621)
#include "switch_gsw.c"
#else
#include "switch_rt305x.c"
#endif