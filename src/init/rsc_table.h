/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 #ifndef RSC_TABLE_H_
 #define RSC_TABLE_H_
 
 #include <stddef.h>
 #include <remoteproc.h>
 
 #if defined __cplusplus
 extern "C" {
 #endif
 
 #define NO_RESOURCE_ENTRIES (2)
 #define RSC_VDEV_FEATURE_NS (1) /* Support name service announcement */
 
//  /* Vring locations and sizes for DDR memory */
//  #define VRING_ALIGN          (0x1000U)
//  #define RL_BUFFER_COUNT      (256U)
//  #define VRING_SIZE           (0x8000U)

#define RESOURCE_TABLE_START 0x90000000U
#define RESOURCE_TABLE_SIZE  0x1000U

void copyResourceTable(void);
 
//  /* DDR memory base for resource table and vrings */
//  #define DDR_BASE             (0x90000000U)
//  #define VDEV0_VRING_BASE     (DDR_BASE + 0x100000U) /* Place vrings after firmware */
//  #define VDEV1_VRING_BASE     (VDEV0_VRING_BASE + 0x20000U)
 
 /* Resource table for the given remote */
 METAL_PACKED_BEGIN
 struct remote_resource_table
 {
     uint32_t version;
     uint32_t num;
     uint32_t reserved[2];
     uint32_t offset[NO_RESOURCE_ENTRIES];
 
     /* rpmsg vdev entry for srtm communication */
     struct fw_rsc_vdev srtm_vdev;
     struct fw_rsc_vdev_vring srtm_vring0;
     struct fw_rsc_vdev_vring srtm_vring1;
     /* rpmsg vdev entry for user app communication */
     struct fw_rsc_vdev user_vdev;
     struct fw_rsc_vdev_vring user_vring0;
     struct fw_rsc_vdev_vring user_vring1;
 } METAL_PACKED_END;
 
 #if defined __cplusplus
 }
 #endif
 
 #endif /* RSC_TABLE_H_ */