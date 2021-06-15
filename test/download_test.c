/*
 * @Author: Caffreyfans
 * @Date: 2021-06-06 12:39:47
 * @LastEditTime: 2021-06-15 23:54:23
 * @Description: stream download test
 */
#include "irbaby.h"

int main(int argc, char *argv[])
{
    t_remote_ac_status status = {
        .ac_power = AC_POWER_ON,
        .ac_temp = AC_TEMP_16,
        .ac_mode = AC_MODE_COOL,
        .ac_wind_dir = AC_SWING_ON,
        .ac_wind_speed = AC_WS_AUTO,
    };
    irbaby_send("new_ac_2582", &status);
    return 0;
}