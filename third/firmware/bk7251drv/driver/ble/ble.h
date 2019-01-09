#ifndef _BLE_H_
#define _BLE_H_

#define BLE_DEBUG

#ifdef BLE_DEBUG
#define BLE_PRT      os_printf
#define BLE_WARN     warning_prf
#define BLE_FATAL    fatal_prf
#else
#define BLE_PRT      null_prf
#define BLE_WARN     null_prf
#define BLE_FATAL    null_prf
#endif

#define BLE_BASE                                     (0x0080B400)

#define BLE_XVR_SLOT_TIME                            (BLE_BASE + 0x2a * 4)
#define BLE_XVR_RF_TX_DELAY_POSI                         (24)
#define BLE_XVR_RF_TX_DELAY_MASK                         (0xFF)
#define BLE_XVR_RF_RX_DELAY_POSI                         (16)
#define BLE_XVR_RF_RX_DELAY_MASK                         (0xFF)
#define BLE_XVR_TX_SLOT_TIME_POSI                        (8)
#define BLE_XVR_TX_SLOT_TIME_MASK                        (0xFF)
#define BLE_XVR_RX_SLOT_TIME_POSI                        (0)
#define BLE_XVR_RX_SLOT_TIME_MASK                        (0xFF)

UINT32 ble_ctrl( UINT32 cmd, void *param );
void ble_isr(void);

#endif /* _BLE_H_ */
