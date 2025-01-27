/*
 * DO NOT include this file anywhere besides runtime_config.c - its format is meant to be internal to the configuration
 * parsing.
 */
#ifndef REDPILLLKM_PLATFORMS_H
#define REDPILLLKM_PLATFORMS_H

#include "../shim/pci_shim.h"
#include "platform_types.h"

#if defined(RP_PLATFORM_APOLLOLAKE)
const struct hw_config platformConfig = {
    .name = "", // "DS218+ DS418play DS718+ DS918+ DS1019+ DS620slim",
    .pci_stubs = {
        {.type = __VPD_TERMINATOR__}},
    .emulate_rtc = true,
    .swap_serial = false,
    .reinit_ttyS0 = true,
    .fix_disk_led_ctrl = true,
    .has_cpu_temp = true,
    .is_dt = false,
    .hwmon = {
        .sys_thermal = {HWMON_SYS_TZONE_NULL_ID},
        .sys_voltage = {HWMON_SYS_VSENS_NULL_ID},
        .sys_fan_speed_rpm = {HWMON_SYS_FAN_NULL_ID},
        .hdd_backplane = {HWMON_SYS_HDD_BP_DETECT_ID, HWMON_SYS_HDD_BP_ENABLE_ID},
        .psu_status = {HWMON_PSU_NULL_ID},
        .sys_current = {HWMON_SYS_CURR_NULL_ID},
    }};
#elif defined(RP_PLATFORM_BROADWELL)
const struct hw_config platformConfig = {
    .name = "", // "DS3617xs DS3617xsII RS3617RPxs RS3617xs+ RS4017xs+ RS18017xs+ RS3618xs FS2017 FS3400",
    .pci_stubs = {
        {.type = __VPD_TERMINATOR__}},
    .emulate_rtc = false,
    .swap_serial = false,
    .reinit_ttyS0 = true,
    .fix_disk_led_ctrl = false,
    .has_cpu_temp = true,
    .is_dt = false,
    .hwmon = {
        .sys_thermal = {HWMON_SYS_TZONE_REMOTE1_ID, HWMON_SYS_TZONE_LOCAL_ID, HWMON_SYS_TZONE_REMOTE2_ID},
        .sys_voltage = {HWMON_SYS_VSENS_VCC_ID, HWMON_SYS_VSENS_VPP_ID, HWMON_SYS_VSENS_V33_ID, HWMON_SYS_VSENS_V5_ID, HWMON_SYS_VSENS_V12_ID},
        .sys_fan_speed_rpm = {HWMON_SYS_FAN1_ID, HWMON_SYS_FAN2_ID},
        .hdd_backplane = {HWMON_SYS_HDD_BP_NULL_ID},
        .psu_status = {HWMON_PSU_NULL_ID},
        .sys_current = {HWMON_SYS_CURR_NULL_ID},
    }};
#elif defined(RP_PLATFORM_BROADWELLNK)
const struct hw_config platformConfig = {
    .name = "", // "DS3018xs RS1619xs+ DS1621xs+ RS3621RPxs RS3621xs+ RS4021xs+ DS3622xs+ SA3400 SA3600 FS1018 FS3600",
    .pci_stubs = {
        {.type = __VPD_TERMINATOR__}},
    .emulate_rtc = false,
    .swap_serial = false,
    .reinit_ttyS0 = true,
    .fix_disk_led_ctrl = true,
    .has_cpu_temp = true,
    .is_dt = false,
    .hwmon = {
        .sys_thermal = {HWMON_SYS_TZONE_REMOTE1_ID, HWMON_SYS_TZONE_LOCAL_ID, HWMON_SYS_TZONE_REMOTE2_ID},
        .sys_voltage = {HWMON_SYS_VSENS_VCC_ID, HWMON_SYS_VSENS_VPP_ID, HWMON_SYS_VSENS_V33_ID, HWMON_SYS_VSENS_V5_ID, HWMON_SYS_VSENS_V12_ID},
        .sys_fan_speed_rpm = {HWMON_SYS_FAN1_ID, HWMON_SYS_FAN2_ID},
        .hdd_backplane = {HWMON_SYS_HDD_BP_NULL_ID},
        .psu_status = {HWMON_PSU_NULL_ID},
        .sys_current = {HWMON_SYS_CURR_NULL_ID},
    }};
#elif defined(RP_PLATFORM_BROADWELLNKV2)
const struct hw_config platformConfig = {
    .name = "", // "SA3410 SA3610 FS3410",
    .pci_stubs = {
        {.type = __VPD_TERMINATOR__}},
    .emulate_rtc = false,
    .swap_serial = false,
    .reinit_ttyS0 = true,
    .fix_disk_led_ctrl = true,
    .has_cpu_temp = true,
    .is_dt = false,
    .hwmon = {
        .sys_thermal = {HWMON_SYS_TZONE_REMOTE1_ID, HWMON_SYS_TZONE_LOCAL_ID, HWMON_SYS_TZONE_REMOTE2_ID},
        .sys_voltage = {HWMON_SYS_VSENS_VCC_ID, HWMON_SYS_VSENS_VPP_ID, HWMON_SYS_VSENS_V33_ID, HWMON_SYS_VSENS_V5_ID, HWMON_SYS_VSENS_V12_ID},
        .sys_fan_speed_rpm = {HWMON_SYS_FAN1_ID, HWMON_SYS_FAN2_ID},
        .hdd_backplane = {HWMON_SYS_HDD_BP_NULL_ID},
        .psu_status = {HWMON_PSU_NULL_ID},
        .sys_current = {HWMON_SYS_CURR_NULL_ID},
    }};
#elif defined(RP_PLATFORM_BROADWELLNTBAP)
const struct hw_config platformConfig = {
    .name = "", // "SA3200d SA3400d",
    .pci_stubs = {
        {.type = __VPD_TERMINATOR__}},
    .emulate_rtc = false,
    .swap_serial = false,
    .reinit_ttyS0 = true,
    .fix_disk_led_ctrl = true,
    .has_cpu_temp = true,
    .is_dt = false,
    .hwmon = {
        .sys_thermal = {HWMON_SYS_TZONE_REMOTE1_ID, HWMON_SYS_TZONE_LOCAL_ID, HWMON_SYS_TZONE_REMOTE2_ID},
        .sys_voltage = {HWMON_SYS_VSENS_VCC_ID, HWMON_SYS_VSENS_VPP_ID, HWMON_SYS_VSENS_V33_ID, HWMON_SYS_VSENS_V5_ID, HWMON_SYS_VSENS_V12_ID},
        .sys_fan_speed_rpm = {HWMON_SYS_FAN1_ID, HWMON_SYS_FAN2_ID},
        .hdd_backplane = {HWMON_SYS_HDD_BP_NULL_ID},
        .psu_status = {HWMON_PSU_NULL_ID},
        .sys_current = {HWMON_SYS_CURR_NULL_ID},
    }};
#elif defined(RP_PLATFORM_DENVERTON)
const struct hw_config platformConfig = {
    .name = "", // "DS1618+ RS2418+ RS2418RP+ RS2818RP+ DS1819+ DS2419+ DS2419+II DVA3219 RS820+ RS820RP+ DVA3221",
    .pci_stubs = {
        {.type = __VPD_TERMINATOR__}},
    .emulate_rtc = false,
    .swap_serial = false,
    .reinit_ttyS0 = true,
    .fix_disk_led_ctrl = true,
    .has_cpu_temp = true,
    .is_dt = false,
    .hwmon = {
        .sys_thermal = {HWMON_SYS_TZONE_REMOTE1_ID, HWMON_SYS_TZONE_LOCAL_ID, HWMON_SYS_TZONE_REMOTE2_ID},
        .sys_voltage = {HWMON_SYS_VSENS_VCC_ID, HWMON_SYS_VSENS_VPP_ID, HWMON_SYS_VSENS_V33_ID, HWMON_SYS_VSENS_V5_ID, HWMON_SYS_VSENS_V12_ID},
        .sys_fan_speed_rpm = {HWMON_SYS_FAN1_ID, HWMON_SYS_FAN2_ID},
        .hdd_backplane = {HWMON_SYS_HDD_BP_NULL_ID},
        .psu_status = {HWMON_PSU_NULL_ID},
        .sys_current = {HWMON_SYS_CURR_NULL_ID},
    }};
#elif defined(RP_PLATFORM_GEMINILAKE)
const struct hw_config platformConfig = {
    .name = "", // "DS220+ DS420+ DS720+ DS920+ DS1520+ DVA1622 DS423+ DS224+",
    .pci_stubs = {
        {.type = __VPD_TERMINATOR__}},
    .emulate_rtc = true,
    .swap_serial = false,
    .reinit_ttyS0 = true,
    .fix_disk_led_ctrl = true,
    .has_cpu_temp = true,
    .is_dt = true,
    .hwmon = {
        .sys_thermal = {HWMON_SYS_TZONE_NULL_ID},
        .sys_voltage = {HWMON_SYS_VSENS_NULL_ID},
        .sys_fan_speed_rpm = {HWMON_SYS_FAN_NULL_ID},
        .hdd_backplane = {HWMON_SYS_HDD_BP_DETECT_ID, HWMON_SYS_HDD_BP_ENABLE_ID},
        .psu_status = {HWMON_PSU_NULL_ID},
        .sys_current = {HWMON_SYS_CURR_NULL_ID},
    }};
#elif defined(RP_PLATFORM_PURLEY)
const struct hw_config platformConfig = {
    .name = "", // "HD6500 FS6400",
    .pci_stubs = {
        {.type = __VPD_TERMINATOR__}},
    .emulate_rtc = false,
    .swap_serial = false,
    .reinit_ttyS0 = true,
    .fix_disk_led_ctrl = true,
    .has_cpu_temp = true,
    .is_dt = true,
    .hwmon = {
        .sys_thermal = {HWMON_SYS_TZONE_REMOTE1_ID, HWMON_SYS_TZONE_LOCAL_ID, HWMON_SYS_TZONE_REMOTE2_ID},
        .sys_voltage = {HWMON_SYS_VSENS_VCC_ID, HWMON_SYS_VSENS_VPP_ID, HWMON_SYS_VSENS_V33_ID, HWMON_SYS_VSENS_V5_ID, HWMON_SYS_VSENS_V12_ID},
        .sys_fan_speed_rpm = {HWMON_SYS_FAN1_ID, HWMON_SYS_FAN2_ID},
        .hdd_backplane = {HWMON_SYS_HDD_BP_NULL_ID},
        .psu_status = {HWMON_PSU_NULL_ID},
        .sys_current = {HWMON_SYS_CURR_NULL_ID},
    }};
#elif defined(RP_PLATFORM_R1000)
const struct hw_config platformConfig = {
    .name = "", // "DS1522+ RS422+ DS723+ DS923+",
    .pci_stubs = {
        {.type = __VPD_TERMINATOR__}},
    .emulate_rtc = false,
    .swap_serial = false,
    .reinit_ttyS0 = true,
    .fix_disk_led_ctrl = true,
    .has_cpu_temp = true,
    .is_dt = true,
    .hwmon = {
        .sys_thermal = {HWMON_SYS_TZONE_REMOTE1_ID, HWMON_SYS_TZONE_LOCAL_ID, HWMON_SYS_TZONE_REMOTE2_ID},
        .sys_voltage = {HWMON_SYS_VSENS_VCC_ID, HWMON_SYS_VSENS_VPP_ID, HWMON_SYS_VSENS_V33_ID, HWMON_SYS_VSENS_V5_ID, HWMON_SYS_VSENS_V12_ID},
        .sys_fan_speed_rpm = {HWMON_SYS_FAN1_ID, HWMON_SYS_FAN2_ID},
        .hdd_backplane = {HWMON_SYS_HDD_BP_NULL_ID},
        .psu_status = {HWMON_PSU_NULL_ID},
        .sys_current = {HWMON_SYS_CURR_NULL_ID},
    }};
#elif defined(RP_PLATFORM_V1000)
const struct hw_config platformConfig = {
    .name = "", // "DS1621+ DS1821+ RS1221+ RS1221RP+ RS2421+ RS2421RP+ RS2821RP+ DS2422+ RS822+ RS822RP+ DS1823xs+ RS2423+ RS2423rp+ FS2500",
    .pci_stubs = {
        {.type = __VPD_TERMINATOR__}},
    .emulate_rtc = false,
    .swap_serial = false,
    .reinit_ttyS0 = true,
    .fix_disk_led_ctrl = true,
    .has_cpu_temp = true,
    .is_dt = true,
    .hwmon = {
        .sys_thermal = {HWMON_SYS_TZONE_REMOTE1_ID, HWMON_SYS_TZONE_LOCAL_ID, HWMON_SYS_TZONE_REMOTE2_ID},
        .sys_voltage = {HWMON_SYS_VSENS_VCC_ID, HWMON_SYS_VSENS_VPP_ID, HWMON_SYS_VSENS_V33_ID, HWMON_SYS_VSENS_V5_ID, HWMON_SYS_VSENS_V12_ID},
        .sys_fan_speed_rpm = {HWMON_SYS_FAN1_ID, HWMON_SYS_FAN2_ID},
        .hdd_backplane = {HWMON_SYS_HDD_BP_NULL_ID},
        .psu_status = {HWMON_PSU_NULL_ID},
        .sys_current = {HWMON_SYS_CURR_NULL_ID},
    }};
#elif defined(RP_PLATFORM_EPYC7002)
const struct hw_config platformConfig = {
    .name = "", // "SA6400",
    .pci_stubs = {
        {.type = __VPD_TERMINATOR__}},
    .emulate_rtc = true,
    .swap_serial = false,
    .reinit_ttyS0 = false,
    .fix_disk_led_ctrl = true,
    .has_cpu_temp = true,
    .is_dt = true,
    .hwmon = {
        .sys_thermal = {HWMON_SYS_TZONE_REMOTE1_ID, HWMON_SYS_TZONE_LOCAL_ID, HWMON_SYS_TZONE_REMOTE2_ID},
        .sys_voltage = {HWMON_SYS_VSENS_VCC_ID, HWMON_SYS_VSENS_VPP_ID, HWMON_SYS_VSENS_V33_ID, HWMON_SYS_VSENS_V5_ID, HWMON_SYS_VSENS_V12_ID},
        // FIXME add NULL ID to workaroud scemd coredump in SA6400 7.2-64551
        .sys_fan_speed_rpm = {HWMON_SYS_FAN1_ID, HWMON_SYS_FAN2_ID, HWMON_SYS_FAN_NULL_ID},
        .hdd_backplane = {HWMON_SYS_HDD_BP_DETECT_ID, HWMON_SYS_HDD_BP_NULL_ID},
        .psu_status = {HWMON_PSU_NULL_ID},
        .sys_current = {HWMON_SYS_CURR_NULL_ID},
    }};
#elif defined(RP_PLATFORM_KVMX64)
const struct hw_config platformConfig = {
    .name = "", // "VirtualDSM",
    .pci_stubs = {
        {.type = __VPD_TERMINATOR__}},
    .emulate_rtc = true,
    .swap_serial = false,
    .reinit_ttyS0 = true,
    .fix_disk_led_ctrl = true,
    .has_cpu_temp = true,
    .is_dt = true,
    .hwmon = {
        .sys_thermal = {HWMON_SYS_TZONE_NULL_ID},
        .sys_voltage = {HWMON_SYS_VSENS_NULL_ID},
        .sys_fan_speed_rpm = {HWMON_SYS_FAN_NULL_ID},
        .hdd_backplane = {HWMON_SYS_HDD_BP_DETECT_ID, HWMON_SYS_HDD_BP_ENABLE_ID},
        .psu_status = {HWMON_PSU_NULL_ID},
        .sys_current = {HWMON_SYS_CURR_NULL_ID},
    }};
#else // defined(RP_PLATFORM_UNKNOWN)
const struct hw_config platformConfig = {
    .name = "", // "unknown",
    .pci_stubs = {
        {.type = __VPD_TERMINATOR__}},
    .emulate_rtc = false,
    .swap_serial = false,
    .reinit_ttyS0 = true,
    .fix_disk_led_ctrl = true,
    .has_cpu_temp = true,
    .is_dt = true,
    .hwmon = {
        .sys_thermal = {HWMON_SYS_TZONE_REMOTE1_ID, HWMON_SYS_TZONE_LOCAL_ID, HWMON_SYS_TZONE_REMOTE2_ID},
        .sys_voltage = {HWMON_SYS_VSENS_VCC_ID, HWMON_SYS_VSENS_VPP_ID, HWMON_SYS_VSENS_V33_ID, HWMON_SYS_VSENS_V5_ID, HWMON_SYS_VSENS_V12_ID},
        .sys_fan_speed_rpm = {HWMON_SYS_FAN1_ID, HWMON_SYS_FAN2_ID},
        .hdd_backplane = {HWMON_SYS_HDD_BP_NULL_ID},
        .psu_status = {HWMON_PSU_NULL_ID},
        .sys_current = {HWMON_SYS_CURR_NULL_ID},
    }}
#endif

#endif // REDPILLLKM_PLATFORMS_H
