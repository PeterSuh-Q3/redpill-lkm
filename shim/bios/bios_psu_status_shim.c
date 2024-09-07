/**
 * Overrides HWMONGetPSUStatusByI2C to provide fake psu status for SA6400, RS4021xsp and FS2500
 * to find the required symbol you can consult the mfgbios output while booting the dev LKM
 */
#include "bios_psu_status_shim.h"
#include "../../common.h"
#include "../shim_base.h"
#include "../../internal/override/override_symbol.h" //overriding HWMONGetPSUStatusByI2C
#include "../../config/platform_types.h" //hw_config, platform_has_hwmon_*
#include <linux/synobios.h> //CAPABILITY_*, CAPABILITY

#define SHIM_NAME "mfgBIOS HWMONGetPSUStatusByI2C"

static const struct hw_config *hw_config = NULL;
static override_symbol_inst *HWMONGetPSUStatusByI2C_ovs = NULL;
static override_symbol_inst *RS4021xspI2CGetPowerInfo_ovs = NULL;
static override_symbol_inst *RS4022xspI2CGetPowerInfo_ovs = NULL;
static override_symbol_inst *RS4023xspI2CGetPowerInfo_ovs = NULL;
static override_symbol_inst *RS4024xspI2CGetPowerInfo_ovs = NULL;
static override_symbol_inst *FS3410I2CGetPowerInfo_ovs = NULL;
static override_symbol_inst *FS6400I2CGetPowerInfo_ovs = NULL;
static override_symbol_inst *FS6500I2CGetPowerInfo_ovs = NULL;
static override_symbol_inst *HD6500I2CGetPowerInfo_ovs = NULL;
static override_symbol_inst *SA6500I2CGetPowerInfo_ovs = NULL;

static int HWMONGetPSUStatusByI2C_shim(void)
{
    return 0;
}

static int HWMONI2CGetPowerInfo_shim(void)
{
    return 0;
}

int I2CSmbusReadPowerStatus(void)
{
    return 0;
}

int register_bios_psu_status_shim(const struct hw_config *hw)
{
    shim_reg_in();

    if (unlikely(HWMONGetPSUStatusByI2C_ovs))
        shim_reg_already();
    if (unlikely(RS4021xspI2CGetPowerInfo_ovs))
        shim_reg_already();
    if (unlikely(RS4022xspI2CGetPowerInfo_ovs))
        shim_reg_already();
    if (unlikely(RS4023xspI2CGetPowerInfo_ovs))
        shim_reg_already();
    if (unlikely(RS4024xspI2CGetPowerInfo_ovs))
        shim_reg_already();
    if (unlikely(FS3410I2CGetPowerInfo_ovs))
        shim_reg_already();
    if (unlikely(FS6400I2CGetPowerInfo_ovs))
        shim_reg_already();
    if (unlikely(FS6500I2CGetPowerInfo_ovs))
        shim_reg_already();
    if (unlikely(HD6500I2CGetPowerInfo_ovs))
        shim_reg_already();
    if (unlikely(SA6500I2CGetPowerInfo_ovs))
        shim_reg_already();
        
    hw_config = hw;
    override_symbol_or_exit_int(HWMONGetPSUStatusByI2C_ovs, "HWMONGetPSUStatusByI2C", HWMONGetPSUStatusByI2C_shim);
    override_symbol_or_exit_int(RS4021xspI2CGetPowerInfo_ovs, "RS4021xspI2CGetPowerInfo", HWMONI2CGetPowerInfo_shim);
    override_symbol_or_exit_int(RS4022xspI2CGetPowerInfo_ovs, "RS4022xspI2CGetPowerInfo", HWMONI2CGetPowerInfo_shim);
    override_symbol_or_exit_int(RS4023xspI2CGetPowerInfo_ovs, "RS4023xspI2CGetPowerInfo", HWMONI2CGetPowerInfo_shim);
    override_symbol_or_exit_int(RS4024xspI2CGetPowerInfo_ovs, "RS4024xspI2CGetPowerInfo", HWMONI2CGetPowerInfo_shim);
    override_symbol_or_exit_int(FS3410I2CGetPowerInfo_ovs, "FS3410I2CGetPowerInfo", HWMONI2CGetPowerInfo_shim);
    override_symbol_or_exit_int(FS6400I2CGetPowerInfo_ovs, "FS6400I2CGetPowerInfo", HWMONI2CGetPowerInfo_shim);
    override_symbol_or_exit_int(FS6500I2CGetPowerInfo_ovs, "FS6500I2CGetPowerInfo", HWMONI2CGetPowerInfo_shim);
    override_symbol_or_exit_int(HD6500I2CGetPowerInfo_ovs, "HD6500I2CGetPowerInfo", HWMONI2CGetPowerInfo_shim);
    override_symbol_or_exit_int(SA6500I2CGetPowerInfo_ovs, "SA6500I2CGetPowerInfo", HWMONI2CGetPowerInfo_shim);
    shim_reg_ok();
    return 0;
}

int unregister_bios_psu_status_shim(void)
{
    int out = 0;
    shim_ureg_in();

    if (unlikely(!HWMONGetPSUStatusByI2C_ovs))
        return 0; //this is deliberately a noop
    if (unlikely(!RS4021xspI2CGetPowerInfo_ovs))
        return 0; //this is deliberately a noop
    if (unlikely(!RS4022xspI2CGetPowerInfo_ovs))
        return 0; //this is deliberately a noop
    if (unlikely(!RS4023xspI2CGetPowerInfo_ovs))
        return 0; //this is deliberately a noop
    if (unlikely(!RS4024xspI2CGetPowerInfo_ovs))
        return 0; //this is deliberately a noop
    if (unlikely(!FS3410I2CGetPowerInfo_ovs))
        return 0; //this is deliberately a noop
    if (unlikely(!FS6400I2CGetPowerInfo_ovs))
        return 0; //this is deliberately a noop
    if (unlikely(!FS6500I2CGetPowerInfo_ovs))
        return 0; //this is deliberately a noop
    if (unlikely(!HD6500I2CGetPowerInfo_ovs))
        return 0; //this is deliberately a noop
    if (unlikely(!SA6500I2CGetPowerInfo_ovs))
        return 0; //this is deliberately a noop

    out = restore_symbol(HWMONGetPSUStatusByI2C_ovs);
    if (unlikely(out != 0)) {
        pr_loc_err("Failed to restore HWMONGetPSUStatusByI2C_ovs - error=%d", out);
        return out;
    }
    HWMONGetPSUStatusByI2C_ovs = NULL;

    out = restore_symbol(RS4021xspI2CGetPowerInfo_ovs);
    if (unlikely(out != 0)) {
        pr_loc_err("Failed to restore RS4021xspI2CGetPowerInfo_ovs - error=%d", out);
        return out;
    }
    RS4021xspI2CGetPowerInfo_ovs = NULL;

    out = restore_symbol(RS4022xspI2CGetPowerInfo_ovs);
    if (unlikely(out != 0)) {
        pr_loc_err("Failed to restore RS4022xspI2CGetPowerInfo_ovs - error=%d", out);
        return out;
    }
    RS4022xspI2CGetPowerInfo_ovs = NULL;

    out = restore_symbol(RS4023xspI2CGetPowerInfo_ovs);
    if (unlikely(out != 0)) {
        pr_loc_err("Failed to restore RS4023xspI2CGetPowerInfo_ovs - error=%d", out);
        return out;
    }
    RS4023xspI2CGetPowerInfo_ovs = NULL;

    out = restore_symbol(RS4024xspI2CGetPowerInfo_ovs);
    if (unlikely(out != 0)) {
        pr_loc_err("Failed to restore RS4024xspI2CGetPowerInfo_ovs - error=%d", out);
        return out;
    }
    RS4024xspI2CGetPowerInfo_ovs = NULL;

    out = restore_symbol(FS3410I2CGetPowerInfo_ovs);
    if (unlikely(out != 0)) {
        pr_loc_err("Failed to restore FS3410I2CGetPowerInfo_ovs - error=%d", out);
        return out;
    }
    FS3410I2CGetPowerInfo_ovs = NULL;

    out = restore_symbol(FS6400I2CGetPowerInfo_ovs);
    if (unlikely(out != 0)) {
        pr_loc_err("Failed to restore FS6400I2CGetPowerInfo_ovs - error=%d", out);
        return out;
    }
    FS6400I2CGetPowerInfo_ovs = NULL;

    out = restore_symbol(FS6500I2CGetPowerInfo_ovs);
    if (unlikely(out != 0)) {
        pr_loc_err("Failed to restore FS6500I2CGetPowerInfo_ovs - error=%d", out);
        return out;
    }
    FS6500I2CGetPowerInfo_ovs = NULL;

    out = restore_symbol(HD6500I2CGetPowerInfo_ovs);
    if (unlikely(out != 0)) {
        pr_loc_err("Failed to restore HD6500I2CGetPowerInfo_ovs - error=%d", out);
        return out;
    }
    HD6500I2CGetPowerInfo_ovs = NULL;

    out = restore_symbol(SA6500I2CGetPowerInfo_ovs);
    if (unlikely(out != 0)) {
        pr_loc_err("Failed to restore SA6500I2CGetPowerInfo_ovs - error=%d", out);
        return out;
    }
    SA6500I2CGetPowerInfo_ovs = NULL;

    shim_ureg_ok();
    return 0;
}

int reset_bios_psu_status_shim(void)
{
    shim_reset_in();
    put_overridden_symbol(HWMONGetPSUStatusByI2C_ovs);
    HWMONGetPSUStatusByI2C_ovs = NULL;

    put_overridden_symbol(RS4021xspI2CGetPowerInfo_ovs);
    RS4021xspI2CGetPowerInfo_ovs = NULL;
    put_overridden_symbol(RS4022xspI2CGetPowerInfo_ovs);
    RS4022xspI2CGetPowerInfo_ovs = NULL;
    put_overridden_symbol(RS4023xspI2CGetPowerInfo_ovs);
    RS4023xspI2CGetPowerInfo_ovs = NULL;
    put_overridden_symbol(RS4024xspI2CGetPowerInfo_ovs);
    RS4024xspI2CGetPowerInfo_ovs = NULL;
    put_overridden_symbol(FS3410I2CGetPowerInfo_ovs);
    FS3410I2CGetPowerInfo_ovs = NULL;
    put_overridden_symbol(FS6400I2CGetPowerInfo_ovs);
    FS6400I2CGetPowerInfo_ovs = NULL;
    put_overridden_symbol(FS6500I2CGetPowerInfo_ovs);
    FS6500I2CGetPowerInfo_ovs = NULL;
    put_overridden_symbol(HD6500I2CGetPowerInfo_ovs);
    HD6500I2CGetPowerInfo_ovs = NULL;
    put_overridden_symbol(SA6500I2CGetPowerInfo_ovs);
    SA6500I2CGetPowerInfo_ovs = NULL;
    
    shim_reset_ok();
    return 0;
}
