#include "boot_shim_base.h"
#include "../../common.h"
#include "../../config/runtime_config.h" //struct boot_media
#include "../../internal/scsi/scsi_toolbox.h" //is_sata_disk(), opportunistic_read_capacity()
#include <scsi/scsi_device.h> //struct scsi_device
#include <linux/usb.h> //struct usb_device
#include <linux/genhd.h>
#include <linux/fs.h>
#include <linux/printk.h>

//Definition of known VID/PIDs for USB-based shims
#define SBOOT_RET_VID 0xf400 //Retail boot drive VID
#define SBOOT_RET_PID 0xf400 //Retail boot drive PID
#define SBOOT_MFG_VID 0xf401 //Force-reinstall boot drive VID
#define SBOOT_MFG_PID 0xf401 //Force-reinstall boot drive PID

void *mapped_shim_data = NULL;

void set_shimmed_boot_dev(void *private_data)
{
    mapped_shim_data = private_data;
}

void *get_shimmed_boot_dev(void)
{
    return mapped_shim_data;
}

bool scsi_is_boot_dev_target(const struct boot_media *boot_dev_config, struct scsi_device *sdp)
{
    if (!is_sata_disk(&sdp->sdev_gendev)) {
        pr_loc_dbg("%s: it's not a SATA disk, ignoring", __FUNCTION__);
        return false;
    }

    pr_loc_dbg("Checking if SATA disk is a shim target - id=%u channel=%u vendor=\"%s\" model=\"%s\"", sdp->id,
               sdp->channel, sdp->vendor, sdp->model);
/*
    long long capacity_mib = opportunistic_read_capacity(sdp);
    if (unlikely(capacity_mib < 0)) {
        pr_loc_dbg("Failed to estimate drive capacity (error=%lld) - it WILL NOT be shimmed", capacity_mib);
        return false;
    }

    if (capacity_mib > boot_dev_config->dom_size_mib) {
        pr_loc_dbg("Device has capacity of ~%llu MiB - it WILL NOT be shimmed (>%lu)", capacity_mib,
                   boot_dev_config->dom_size_mib);
        return false;
    }
*/
    if (!is_loader_disk(sdp)) {
        pr_loc_dbg("%s: it's not a Redpill Loader disk, ignoring", __FUNCTION__);
        return false;
    }
    
    if (unlikely(get_shimmed_boot_dev())) {
        pr_loc_wrn("Boot device was already shimmed but a new matching device appeared again - "
                   "this may produce unpredictable outcomes! Ignoring - check your hardware");
        return false;
    }
/*
    if (unlikely(get_shimmed_boot_dev())) {
        pr_loc_wrn("Boot device was already shimmed but a new matching device (~%llu MiB <= %lu) appeared again - "
                   "this may produce unpredictable outcomes! Ignoring - check your hardware", capacity_mib,
                   boot_dev_config->dom_size_mib);
        return false;
    }

    pr_loc_dbg("Device has capacity of ~%llu MiB - it is a shimmable target (<=%lu)", capacity_mib,
               boot_dev_config->dom_size_mib);
*/
    pr_loc_dbg("Device has 3 vfat partitions (83 Linux)");
    
    return true;
}

void usb_shim_as_boot_dev(const struct boot_media *boot_dev_config, struct usb_device *usb_device)
{
    if (boot_dev_config->mfg_mode) {
        usb_device->descriptor.idVendor = cpu_to_le16(SBOOT_MFG_VID);
        usb_device->descriptor.idProduct = cpu_to_le16(SBOOT_MFG_PID);
    } else {
        usb_device->descriptor.idVendor = cpu_to_le16(SBOOT_RET_VID);
        usb_device->descriptor.idProduct = cpu_to_le16(SBOOT_RET_PID);
    }
}

bool is_loader_disk(struct scsi_device *sdp) {
    struct hd_struct *part;
    struct gendisk *gd;
    int vfat_count = 0;

    // Get the gendisk structure for the SCSI disk
    if (!sdp->request_queue) {
        pr_loc_dbg("sdp->request_queue is null");
        return false;
    }

    gd = (struct gendisk *)sdp->request_queue->queuedata;
    if (!gd) {
        pr_loc_dbg("sdp->request_queue->queuedata is null");
        return false;
    }

    // Scan each partition and count VFAT partitions
    for (int i = 0; i < gd->minors; ++i) {
        part = disk_get_part(gd, i + 1);
        if (!part) {
            continue;
        }

        printk(KERN_INFO "Partition %d type: %d\n", i + 1, part->partno);

        if (part->partno == 0x83) {
            vfat_count++;
        }
    }

    // Check if there are exactly 3 VFAT partitions
    return vfat_count == 3;
}

