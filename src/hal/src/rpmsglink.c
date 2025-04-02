#include <stdbool.h>
#include <string.h>

#include "config.h"
#include "rpmsglink.h"
#include "crtp.h"
#include "configblock.h"
#include "ledseq.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "queuemonitor.h"
#include "semphr.h"

// Include RPMsg lite headers
#include "rpmsg_lite.h"
#include "rpmsg_queue.h"
#include "rpmsg_ns.h"
#include "app.h"
#include "board.h" 

#include "fsl_debug_console.h"

// RPMsg specific definitions
#define RPMSG_ENDPOINT_ADDR       (40)
#define RPMSG_CHANNEL_NAME        "rpmsg-crazyflie-channel"

static bool isInit = false;
static xQueueHandle crtpPacketDelivery;
static uint8_t sendBuffer[64];

// RPMsg context objects
static struct rpmsg_lite_instance *rpmsg_instance;
static struct rpmsg_lite_endpoint *rpmsg_endpoint;
static rpmsg_queue_handle rpmsg_queue;

static int rpmsglinkSendPacket(CRTPPacket *p);
static int rpmsglinkSetEnable(bool enable);
static int rpmsglinkReceiveCRTPPacket(CRTPPacket *p);

static struct crtpLinkOperations rpmsglinkOp = {
  .setEnable    = rpmsglinkSetEnable,
  .sendPacket   = rpmsglinkSendPacket,
  .receivePacket = rpmsglinkReceiveCRTPPacket,
};


// Command handler for RPMsg link
void rpmsglinkHandleCommand(uint8_t command) {
    if (command == 0x03) {
      // Command to use RPMsg link for CRTP
      crtpSetLink(rpmsglinkGetLink());
    } 
  }


// Callback function that receives messages from RPMsg
static int32_t rpmsg_recv_callback(void *payload, uint32_t payload_len, uint32_t src, void *priv) {
    // Check if this is a command message
    if (payload_len == 1) {
        // This is a 1-byte message, likely a command
        uint8_t *cmd = (uint8_t*)payload;
        rpmsglinkHandleCommand(*cmd);
        return RL_RELEASE;
    }

    CRTPPacket p;

    if (payload_len > 0) {
        p.size = payload_len - 1;
        memcpy(&p.raw, payload, payload_len);
        xQueueSend(crtpPacketDelivery, &p, 0);
    }
  
  return RL_RELEASE; // Let RPMsg-Lite release the buffer
}

static void rpmsglinkTask(void *param) {
  char buffer[64];  // Local buffer for received data
  char *received_data = buffer;
  uint32_t src, size;
  
  while (1) {
    // Block waiting for data from RPMsg
    if (rpmsg_queue_recv(rpmsg_instance, rpmsg_queue, &src, received_data, 
                         sizeof(buffer), &size, RL_BLOCK) == RL_SUCCESS) {
                         
      CRTPPacket p;
      p.size = size - 1;
      memcpy(&p.raw, received_data, size);
      
      // Release the RPMsg buffer
      memcpy(&p.raw, received_data, size);
      
      // Queue the packet for the CRTP system
      xQueueSend(crtpPacketDelivery, &p, 0);
      
      // Indicate activity
      //ledseqRun(LINK_LED, seq_linkup);
    }
  }
}

static int rpmsglinkReceiveCRTPPacket(CRTPPacket *p) {
  if (xQueueReceive(crtpPacketDelivery, p, M2T(100)) == pdTRUE) {
    ledseqRun(LINK_LED, seq_linkup);
    return 0;
  }
  
  return -1;
}

static int rpmsglinkSendPacket(CRTPPacket *p) {
  int dataSize;
  
  ASSERT(p->size < CRTP_MAX_DATA_SIZE);
  
  sendBuffer[0] = p->header;
  
  if (p->size <= CRTP_MAX_DATA_SIZE) {
    memcpy(&sendBuffer[1], p->data, p->size);
  }
  dataSize = p->size + 1;
  
  // Send the packet via RPMsg
  if (rpmsg_lite_send(rpmsg_instance, rpmsg_endpoint, RPMSG_ENDPOINT_ADDR,
                      (char*)sendBuffer, dataSize, RL_BLOCK) == RL_SUCCESS) {
    ledseqRun(LINK_DOWN_LED, seq_linkup);
    return 0;
  }
  
  return -1;
}

static int rpmsglinkSetEnable(bool enable) {
  // Nothing to do here for this implementation
  return 0;
}

/*
 * Public functions
 */
// void rpmsglinkInit() {
//   if (isInit)
//     return;
  
//   PRINTF("RPMSGLINK: Initializing RPMsg link\n");
//   // Initialize the RPMsg system
//   rpmsg_instance = rpmsg_lite_remote_init(
//       (void*)RPMSG_LITE_SHMEM_BASE, 
//       RPMSG_LITE_LINK_ID,
//       RL_NO_FLAGS);
      
//   if (!rpmsg_instance) {
//     return; // Failed to initialize RPMsg
//   crtpSetLink(rpmsglinkGetLink());
//   }
  
//   // Wait for the A55 core to be ready
//   rpmsg_lite_wait_for_link_up(rpmsg_instance, RL_BLOCK);
  
//   // Create RPMsg queue
//   rpmsg_queue = rpmsg_queue_create(rpmsg_instance);
//   if (!rpmsg_queue) {
//     return; // Failed to create queue
//   }
  
//   // Create RPMsg endpoint
//   rpmsg_endpoint = rpmsg_lite_create_ept(
//       rpmsg_instance, 
//       RPMSG_ENDPOINT_ADDR,
//       rpmsg_recv_callback,
//       rpmsg_queue);
      
//   if (!rpmsg_endpoint) {
//     return; // Failed to create endpoint
//   }
  
//   // Announce our service to the other core
//   rpmsg_ns_announce(rpmsg_instance, rpmsg_endpoint, 
//                    RPMSG_CHANNEL_NAME, RL_NS_CREATE);
  
//   // Create CRTP packet delivery queue
//   crtpPacketDelivery = xQueueCreate(5, sizeof(CRTPPacket));
//   DEBUG_QUEUE_MONITOR_REGISTER(crtpPacketDelivery);
  
//   // Create a task to handle RPMsg communication
//   xTaskCreate(rpmsglinkTask, RPMSGLINK_TASK_NAME,
//               RPMSGLINK_TASK_STACKSIZE, NULL, RPMSGLINK_TASK_PRI, NULL);
  
//   isInit = true;
// }

void rpmsglinkInit() {
    PRINTF("RPMSGLINK: Starting initialization\n");
    
    if (isInit) {
      PRINTF("RPMSGLINK: Already initialized, skipping\n");
      return;
    }
    
    PRINTF("RPMSGLINK: Creating RPMsg instance with SHMEM_BASE=0x%X, LINK_ID=%d\n", 
           (unsigned int)RPMSG_LITE_SHMEM_BASE, RPMSG_LITE_LINK_ID);
    
    // Initialize the RPMsg system
    rpmsg_instance = rpmsg_lite_remote_init(
        (void*)RPMSG_LITE_SHMEM_BASE, 
        RPMSG_LITE_LINK_ID,
        RL_NO_FLAGS);
        
    if (!rpmsg_instance) {
      PRINTF("RPMSGLINK: Failed to initialize RPMsg instance!\n");
      return; // Failed to initialize RPMsg
    }
    
    PRINTF("RPMSGLINK: RPMsg instance created successfully\n");
    
    PRINTF("RPMSGLINK: Waiting for A55 core link up...\n");
    // Wait for the A55 core to be ready
    rpmsg_lite_wait_for_link_up(rpmsg_instance, RL_BLOCK);
    PRINTF("RPMSGLINK: A55 core is now ready\n");
    
    // Create RPMsg queue
    PRINTF("RPMSGLINK: Creating RPMsg queue\n");
    rpmsg_queue = rpmsg_queue_create(rpmsg_instance);
    if (!rpmsg_queue) {
      PRINTF("RPMSGLINK: Failed to create RPMsg queue!\n");
      return; // Failed to create queue
    }
    PRINTF("RPMSGLINK: RPMsg queue created successfully\n");
    
    // Create RPMsg endpoint
    PRINTF("RPMSGLINK: Creating endpoint with address %d\n", RPMSG_ENDPOINT_ADDR);
    rpmsg_endpoint = rpmsg_lite_create_ept(
        rpmsg_instance, 
        RPMSG_ENDPOINT_ADDR,
        rpmsg_recv_callback,
        rpmsg_queue);
        
    if (!rpmsg_endpoint) {
      PRINTF("RPMSGLINK: Failed to create RPMsg endpoint!\n");
      return; // Failed to create endpoint
    }
    PRINTF("RPMSGLINK: RPMsg endpoint created successfully\n");
    
    // Announce our service to the other core
    PRINTF("RPMSGLINK: Announcing service '%s'\n", RPMSG_CHANNEL_NAME);
    rpmsg_ns_announce(rpmsg_instance, rpmsg_endpoint, 
                     RPMSG_CHANNEL_NAME, RL_NS_CREATE);
    
    // Create CRTP packet delivery queue
    PRINTF("RPMSGLINK: Creating CRTP packet delivery queue\n");
    crtpPacketDelivery = xQueueCreate(5, sizeof(CRTPPacket));
    DEBUG_QUEUE_MONITOR_REGISTER(crtpPacketDelivery);
    
    // Create a task to handle RPMsg communication
    PRINTF("RPMSGLINK: Creating RPMsg task\n");
    xTaskCreate(rpmsglinkTask, RPMSGLINK_TASK_NAME,
                RPMSGLINK_TASK_STACKSIZE, NULL, RPMSGLINK_TASK_PRI, NULL);
    
    PRINTF("RPMSGLINK: Setting as default CRTP link\n");
    crtpSetLink(rpmsglinkGetLink());
    
    PRINTF("RPMSGLINK: Initialization complete\n");
    isInit = true;
}

bool rpmsglinkTest() {
  return isInit;
}

struct crtpLinkOperations * rpmsglinkGetLink() {
  return &rpmsglinkOp;
}