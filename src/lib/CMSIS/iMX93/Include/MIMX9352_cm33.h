#if !defined(MIMX9352_cm33_H_)  /* Check if memory map has not been already included */
#define MIMX9352_cm33_H_

/* Essential peripherals - always included */
#include "PERI_CCM.h"        /* Clock control module */
#include "PERI_RGPIO.h"      /* GPIO */
#include "PERI_LPUART.h"     /* UART for communication */
#include "PERI_SYSPM.h"      /* System power management */
#include "PERI_DMA.h"        /* DMA controller */
#include "PERI_TPM.h"        /* Timer/PWM module */
#include "PERI_LPTMR.h"      /* Low power timer */
#include "PERI_IOMUXC1.h"    /* Pin mux controller */
#include "PERI_GPC_CPU_CTRL.h" /* Power controller */
#include "PERI_GPC_GLOBAL.h"   /* Global power controller */
#include "PERI_PLL.h"        /* Required for PLL_Type definition */
#include "PERI_XCACHE.h"     /* Required for XCACHE_Type definition */
#include "PERI_MU.h"

#if !defined(HITL) || defined(INCLUDE_ALL_PERIPHERALS)
/* Non-essential peripherals - excluded in HITL mode */
#include "PERI_ADC.h"
#include "PERI_ANA_OSC.h"
#include "PERI_AXBS.h"
#include "PERI_BBNSM.h"
#include "PERI_BLK_CTRL_MLMIX.h"
#include "PERI_BLK_CTRL_NICMIX.h"
#include "PERI_BLK_CTRL_NS_AONMIX.h"
#include "PERI_BLK_CTRL_S_AONMIX.h"
#include "PERI_BLK_CTRL_WAKEUPMIX.h"
#include "PERI_CACHE_ECC_MCM.h"
#include "PERI_CAN.h"
#include "PERI_DDRC.h"
#include "PERI_DDRMIX_BLK_CTRL.h"
#include "PERI_DMA4.h"
#include "PERI_ENET.h"
#include "PERI_ENET_QOS.h"
#include "PERI_FLEXIO.h"
#include "PERI_FLEXSPI.h"
#include "PERI_FSB.h"
#include "PERI_I2S.h"
#include "PERI_I3C.h"
#include "PERI_IPC.h"
#include "PERI_ISI.h"
#include "PERI_LCDIF.h"
#include "PERI_LPIT.h"
#include "PERI_MCM.h"
#include "PERI_MEDIAMIX_BLK_CTRL.h"
#include "PERI_MIPI_CSI.h"
#include "PERI_MIPI_DSI.h"
#include "PERI_MU.h"
#include "PERI_NPU.h"
#include "PERI_OCRAM_MECC.h"
#include "PERI_OTFAD.h"
#include "PERI_PDM.h"
#include "PERI_PLL.h"
#include "PERI_PXP.h"
#include "PERI_S3MU.h"
#include "PERI_SEMA42.h"
#include "PERI_SPDIF.h"
#include "PERI_SRC_GENERAL_REG.h"
#include "PERI_SRC_MEM_SLICE.h"
#include "PERI_SRC_MIX_SLICE.h"
#include "PERI_SYS_CTR_COMPARE.h"
#include "PERI_SYS_CTR_CONTROL.h"
#include "PERI_SYS_CTR_READ.h"
#include "PERI_TCM_ECC_MCM.h"
#include "PERI_TMU.h"
#include "PERI_TRDC_MBC0.h"
#include "PERI_TRDC_MBC2.h"
#include "PERI_TRDC_MBC4.h"
#include "PERI_TRGMUX.h"
#include "PERI_TSTMR.h"
#include "PERI_USB.h"
#include "PERI_USBNC.h"
#include "PERI_USDHC.h"
#include "PERI_WAKEUP_AHBRM.h"
#include "PERI_WDOG.h"
#include "PERI_XCACHE.h"
#endif /* !HITL */

#endif  /* #if !defined(MIMX9352_cm33_H_) */