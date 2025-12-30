#include "bios_shims_collection.h"
#include "../../config/platform_types.h"
#include "rtc_proxy.h"
#include "bios_hwmon_shim.h"
#include "../../common.h"
#include "../../internal/helper/symbol_helper.h"     //kernel_has_symbol()
#include "../../internal/override/override_symbol.h" //shimming leds stuff
#include <linux/pci.h>  // pci_get_device(), pci_reset_function()

/********************************************* mfgBIOS LKM static shims ***********************************************/
static unsigned long org_shimmed_entries[VTK_SIZE] = {'\0'};
static unsigned long cust_shimmed_entries[VTK_SIZE] = {'\0'};
static bool vtable_printed = false;

/********************************************* Null shim indices (중복 제거) ********************************************/
static const int null_shim_indices[] = {
    VTK_SET_FAN_STATE,
    VTK_SET_DISK_LED,
    VTK_SET_PWR_LED,
    VTK_SET_GPIO_PIN_BLINK,
    VTK_SET_ALR_LED,
    VTK_SET_BUZ_CLR,
    VTK_SET_CPU_FAN_STATUS,
    VTK_SET_PHY_LED,
    VTK_SET_HDD_ACT_LED,
    VTK_GET_MICROP_ID,
    VTK_SET_MICROP_ID
};

/********************************************* Custom shims (개별 구현 필요) ********************************************/
static int bios_get_power_status(POWER_INFO *power)
{
    power->power_1 = POWER_STATUS_GOOD;
    power->power_2 = POWER_STATUS_GOOD;
    return 0;
}

static int shim_get_gpio_pin_usable(int *pin)
{
    pin[1] = 0;
    return 0;
}

static int shim_set_gpio_pin_usable(int *pin)
{
    pr_loc_dbg("set_gpio pin info 0  %d", pin[0]);
    pr_loc_dbg("set_gpio pin info 1  %d", pin[1]);
    pr_loc_dbg("set_gpio pin info 2  %d", pin[2]);
    pr_loc_dbg("set_gpio pin info 3  %d", pin[3]);
    return 0;
}

static int bios_get_buz_clr(unsigned char *state)
{
    *state = 0;
    return 0;
}

/********************************************* Debug helpers (수정됨) **************************************************/
static void print_debug_symbols(const unsigned long *vtable_end)
{
    if (unlikely(!vtable_start)) {
        pr_loc_dbg("Cannot print - no vtable address");
        return;
    }

    int im = vtable_end - vtable_start;
    pr_loc_dbg("Will print %d bytes of memory from %p", im, vtable_start);

    unsigned long *call_ptr = vtable_start;
    unsigned char *byte_ptr = (unsigned char *)vtable_start;
    for (int i = 0; i < im; i += 8, byte_ptr += 8, call_ptr++) {
        // ✅ 버그 수정: 올바른 바이트 오프셋 접근
        pr_loc_dbg_raw("%02x %02x %02x %02x %02x %02x %02x %02x ",
            byte_ptr[0], byte_ptr[1], byte_ptr[2], byte_ptr[3],
            byte_ptr[4], byte_ptr[5], byte_ptr[6], byte_ptr[7]);
        pr_loc_dbg_raw("[%02d] 0x%03x \t%p\t%pS\n", i/8, i, (void *)(*call_ptr), (void *)(*call_ptr));
    }
    pr_loc_dbg_raw("\n");
    pr_loc_dbg("Finished printing memory at %p", byte_ptr);
}

static void print_vtable_once(const unsigned long *vt_end)
{
    if (!vtable_printed) {
        print_debug_symbols(vt_end);
        vtable_printed = true;
    }
}

/********************************************** mfgBIOS shimming routines *********************************************/
void _shim_bios_module_entry(const unsigned int idx, const void *new_sym_ptr)
{
    if (unlikely(!vtable_start)) {
        pr_loc_bug("%s called without vtable start populated", __FUNCTION__);
        return;
    }

    if (unlikely(idx > VTK_SIZE - 1)) {
        pr_loc_bug("Attempted shim on index %d - out of range", idx);
        return;
    }

    if (cust_shimmed_entries[idx] && cust_shimmed_entries[idx] == vtable_start[idx])
        return;

    pr_loc_dbg("mfgBIOS vtable [%d] originally %ps<%p> will now be %ps<%p>", idx, 
               (void *)vtable_start[idx], (void *)vtable_start[idx], new_sym_ptr, new_sym_ptr);
    org_shimmed_entries[idx] = vtable_start[idx];
    cust_shimmed_entries[idx] = (unsigned long)new_sym_ptr;
    vtable_start[idx] = cust_shimmed_entries[idx];
}

// ✅ PCI 리셋 헬퍼 함수
static void ensure_nic_ready(const struct hw_config *hw)
{
    struct pci_dev *ixgbe_dev = NULL;
    int retry_count = 0;
    
    pr_loc_info("Ensuring NIC hardware ready for shimming...");
    
    // Intel X540/X550 등 10GbE NIC 탐지
    while ((ixgbe_dev = pci_get_device(PCI_VENDOR_ID_INTEL, 
                   PCI_ANY_ID, ixgbe_dev)) != NULL) {
        if (ixgbe_dev->device == 0x1528 ||  // X540
            ixgbe_dev->device == 0x1563 ||  // X550
            ixgbe_dev->device == 0x10fb) {  // X557
            pr_loc_info("Found ixgbe NIC %04x:%04x at %s - resetting",
                       ixgbe_dev->vendor, ixgbe_dev->device, 
                       pci_name(ixgbe_dev));
            
            pci_reset_function(ixgbe_dev);
            msleep(2000);  // ✅ 2초 안정화 대기
            break;
        }
        pci_dev_put(ixgbe_dev);
    }
    
    if (!ixgbe_dev)
        pr_loc_dbg("No ixgbe NIC found - proceeding without reset");
}

/**
 * Main shimming function - 최적화됨
 */
bool shim_bios_module(const struct hw_config *hw, struct module *mod, unsigned long *vt_start, unsigned long *vt_end)
{
    // PCI 리셋 + 지연 (한 번만)
    static bool nic_initialized = false;
    if (!nic_initialized) {
        ensure_nic_ready(hw);
        nic_initialized = true;
    }
    
    if (unlikely(!vt_start || !vt_end)) {
        pr_loc_bug("%s called without vtable start or vt_end populated?!", __FUNCTION__);
        return false;
    }

    vtable_start = vt_start;
    print_vtable_once(vt_end);  // ✅ 1회만 출력

    // ✅ 1. Null shims 일괄 처리 (매크로 중복 제거)
    for (int i = 0; i < ARRAY_SIZE(null_shim_indices); i++) {
        _shim_bios_module_entry(null_shim_indices[i], bios_null_zero_int);
    }

    // ✅ 2. Custom shims (개별)
    _shim_bios_module_entry(VTK_GET_GPIO_PIN, shim_get_gpio_pin_usable);
    _shim_bios_module_entry(VTK_SET_GPIO_PIN, shim_set_gpio_pin_usable);
    _shim_bios_module_entry(VTK_GET_BUZ_CLR, bios_get_buz_clr);
    _shim_bios_module_entry(VTK_GET_PWR_STATUS, bios_get_power_status);

    // ✅ 3. RTC proxy (조건부)
    if (hw->emulate_rtc) {
        pr_loc_dbg("Platform requires RTC proxy - enabling");
        register_rtc_proxy_shim();
        _shim_bios_module_entry(VTK_RTC_GET_TIME, rtc_proxy_get_time);
        _shim_bios_module_entry(VTK_RTC_SET_TIME, rtc_proxy_set_time);
        _shim_bios_module_entry(VTK_RTC_INT_APWR, rtc_proxy_init_auto_power_on);
        _shim_bios_module_entry(VTK_RTC_GET_APWR, rtc_proxy_get_auto_power_on);
        _shim_bios_module_entry(VTK_RTC_SET_APWR, rtc_proxy_set_auto_power_on);
        _shim_bios_module_entry(VTK_RTC_UINT_APWR, rtc_proxy_uinit_auto_power_on);
    }

    shim_bios_module_hwmon_entries(hw);
    return true;
}

bool unshim_bios_module(unsigned long *vt_start, unsigned long *vt_end)
{
    for (int i = 0; i < VTK_SIZE; i++) {
        if (!cust_shimmed_entries[i])
            continue;

        pr_loc_dbg("Restoring vtable [%d] from %ps<%p> to %ps<%p>", i, 
                   (void *)vt_start[i], (void *)vt_start[i], 
                   (void *)org_shimmed_entries[i], (void *)org_shimmed_entries[i]);
        vtable_start[i] = org_shimmed_entries[i];
    }

    reset_bios_shims();
    return true;
}

void reset_bios_shims(void)
{
    memset(org_shimmed_entries, 0, sizeof(org_shimmed_entries));
    memset(cust_shimmed_entries, 0, sizeof(cust_shimmed_entries));
    vtable_printed = false;  // ✅ 리셋 추가
    unregister_rtc_proxy_shim();
    reset_bios_module_hwmon_shim();
}

/********************************************* Disk LED shims (배열화) *************************************************/
#if !defined(CONFIG_SYNO_EPYC7002) && !defined(CONFIG_SYNO_PURLEY)
static override_symbol_inst *ov_funcSYNOSATADiskLedCtrl = NULL;
#endif
static override_symbol_inst *ov_syno_ahci_disk_led_enable = NULL;
static override_symbol_inst *ov_syno_ahci_disk_led_enable_by_port = NULL;

#if !defined(CONFIG_SYNO_EPYC7002) && !defined(CONFIG_SYNO_PURLEY)
extern void *funcSYNOSATADiskLedCtrl;
static int funcSYNOSATADiskLedCtrl_shim(int host_num, SYNO_DISK_LED led)
{
    pr_loc_dbg("Received %s with host=%d led=%d", __FUNCTION__, host_num, led);
    return 0;
}
#endif

static int syno_ahci_disk_led_enable_shim(const unsigned short host_num, const int value)
{
    pr_loc_dbg("Received %s with host=%d val=%d", __FUNCTION__, host_num, value);
    return 0;
}

static int syno_ahci_disk_led_enable_by_port_shim(const unsigned short port, const int value)
{
    pr_loc_dbg("Received %s with port=%d val=%d", __FUNCTION__, port, value);
    return 0;
}

// ✅ 배열로 중복 제거
static override_symbol_inst *disk_led_shims[] = {
#if !defined(CONFIG_SYNO_EPYC7002) && !defined(CONFIG_SYNO_PURLEY)
    &ov_funcSYNOSATADiskLedCtrl,
#endif
    &ov_syno_ahci_disk_led_enable,
    &ov_syno_ahci_disk_led_enable_by_port,
    NULL
};

int shim_disk_leds_ctrl(const struct hw_config *hw)
{
    if (!hw->fix_disk_led_ctrl)
        return 0;

    pr_loc_dbg("Shimming disk led control API");
    int out;

#if !defined(CONFIG_SYNO_EPYC7002) && !defined(CONFIG_SYNO_PURLEY)
    if (funcSYNOSATADiskLedCtrl) {
        ov_funcSYNOSATADiskLedCtrl = override_symbol("funcSYNOSATADiskLedCtrl", funcSYNOSATADiskLedCtrl_shim);
        if (IS_ERR(ov_funcSYNOSATADiskLedCtrl)) {
            out = PTR_ERR(ov_funcSYNOSATADiskLedCtrl);
            ov_funcSYNOSATADiskLedCtrl = NULL;
            pr_loc_err("Failed to shim funcSYNOSATADiskLedCtrl, error=%d", out);
            return out;
        }
    }
#endif

    if (kernel_has_symbol("syno_ahci_disk_led_enable")) {
        ov_syno_ahci_disk_led_enable = override_symbol("syno_ahci_disk_led_enable", syno_ahci_disk_led_enable_shim);
        if (IS_ERR(ov_syno_ahci_disk_led_enable)) {
            out = PTR_ERR(ov_syno_ahci_disk_led_enable);
            ov_syno_ahci_disk_led_enable = NULL;
            pr_loc_err("Failed to shim syno_ahci_disk_led_enable, error=%d", out);
            return out;
        }
    }

    if (kernel_has_symbol("syno_ahci_disk_led_enable_by_port")) {
        ov_syno_ahci_disk_led_enable_by_port = override_symbol("syno_ahci_disk_led_enable_by_port", syno_ahci_disk_led_enable_by_port_shim);
        if (IS_ERR(ov_syno_ahci_disk_led_enable_by_port)) {
            out = PTR_ERR(ov_syno_ahci_disk_led_enable_by_port);
            ov_syno_ahci_disk_led_enable_by_port = NULL;
            pr_loc_err("Failed to shim syno_ahci_disk_led_enable_by_port, error=%d", out);
            return out;
        }
    }

    pr_loc_dbg("Finished %s", __FUNCTION__);
    return 0;
}

// ✅ unshim 로직 배열화 (중복 제거)
int unshim_disk_leds_ctrl(void)
{
    pr_loc_dbg("Unshimming disk led control API");
    int failed = 0;

    for (int i = 0; disk_led_shims[i]; i++) {
        override_symbol_inst **shim = disk_led_shims[i];
        if (*shim) {
            int out = restore_symbol(*shim);
            *shim = NULL;
            if (out != 0) {
                pr_loc_err("Failed to unshim disk_led_shim[%d], error=%d", i, out);
                failed = -EINVAL;
            }
        }
    }

    pr_loc_dbg("Finished %s (exit=%d)", __FUNCTION__, failed);
    return failed;
}
