/**
 ****************************************************************************************
 *
 * @file l2cc_lecb.c
 *
 * @brief L2CAP LE Credit Based connection management
 *
 * Copyright (C) RivieraWaves 2009-2016
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @addtogroup L2CC_LECB
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"

#if (BLE_LECB)
#include "l2cc_int.h"
#include "l2cc_task.h"
#include "l2cc_lecb.h"
#include "common_utils.h"
#include "kernel_timer.h"
#include "gapm.h"
#include "kernel_mem.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * MACROS
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * LOCAL FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */

/*
 * EXPORTED FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */

uint16_t l2cc_lecb_h2l_err(uint8_t h_err)
{
    uint8_t result = L2C_CB_CON_INS_AUTHOR;
    switch (h_err)
    {

        case GAP_ERR_NO_ERROR:
            // No error
            result = L2C_CB_CON_SUCCESS;
            break;

        case L2C_ERR_LEPSM_NOT_SUPP:
            // LEPSM is not supported by peer device
            result = L2C_CB_CON_LEPSM_NOT_SUPP;
            break;

        case L2C_ERR_NO_RES_AVAIL:
            // No resources available
            result = L2C_CB_CON_NO_RES_AVAIL;
            break;

        case L2C_ERR_INSUFF_AUTHEN:
            // Insufficient authentication
            result = L2C_CB_CON_INS_AUTH;
            break;

        case L2C_ERR_INSUFF_AUTHOR :
            // Insufficient authorization
            result = L2C_CB_CON_INS_AUTHOR;
            break;

        case L2C_ERR_INSUFF_ENC_KEY_SIZE:
            // Insufficient encription key size
            result = L2C_CB_CON_INS_EKS;
            break;

        case L2C_ERR_INSUFF_ENC:
            // Insufficient encription
            result = L2C_CB_CON_INS_ENCRYPTION;
            break;

        default:
            // result is L2C_CB_CON_INS_AUTHOR
            break;
    }

    return (result);
}

struct l2cc_lecb_info* l2cc_lecb_find(uint8_t conidx, uint8_t field, uint16_t value)
{
    struct l2cc_lecb_info *lecb = NULL;
    if(l2cc_env[conidx] != NULL)
    {
        bool found = false;

        if (!common_list_is_empty(&(l2cc_env[conidx]->lecb_list)))
        {
            lecb = (struct l2cc_lecb_info *) common_list_pick(&(l2cc_env[conidx]->lecb_list));
            while (lecb != NULL)
            {
                switch (field)
                {
                    case L2CC_LECB_LEPSM:
                    {
                        if (lecb->le_psm == value)
                        {
                            found = true;
                        }
                    } break;

                    case L2CC_LECB_LOCAL_CID:
                    {
                        if (lecb->local_cid == value)
                        {
                            found = true;
                        }
                    } break;

                    case L2CC_LECB_PEER_CID:
                    {
                        if (lecb->peer_cid == value)
                        {
                            found = true;
                        }
                    } break;
                    default: /* nothing to do */ break;
                }

                if (found)
                {
                    break;
                }

                lecb =  (struct l2cc_lecb_info *) lecb->hdr.next;
            }
        }
    }

    return lecb;
}

void l2cc_lecb_free(uint8_t conidx, struct l2cc_lecb_info* lecb, bool disconnect_ind)
{
    struct kernel_msg* msg;
    struct l2cc_env_tag *env = l2cc_env[conidx];

    // extract the LECB connection from list
    common_list_extract(&(env->lecb_list), &lecb->hdr, 0);

    // unregister the link
    gapm_lecb_unregister(lecb->le_psm, GETB(lecb->state, L2CC_LECB_PEER_INIT));

    if(disconnect_ind)
    {
        // send disconnection indication
        struct l2cc_lecb_disconnect_ind* ind = KERNEL_MSG_ALLOC(L2CC_LECB_DISCONNECT_IND,
                lecb->task_id, KERNEL_BUILD_ID(TASK_L2CC, conidx), l2cc_lecb_disconnect_ind);
        ind->le_psm    = lecb->le_psm;
        ind->local_cid = lecb->local_cid;
        ind->peer_cid  = lecb->peer_cid;
        ind->reason    = (lecb->disc_reason == GAP_ERR_NO_ERROR) ? GAP_ERR_DISCONNECTED : lecb->disc_reason;
        kernel_msg_send(ind);
    }

    // check if there is something in RX queue
    if(lecb->rx_sdu != NULL)
    {
        msg = kernel_param2msg(lecb->rx_sdu);

        // if a message is on-going
        if(env->rx_buffer == msg)
        {
            // remove rx message
            env->rx_buffer = NULL;
        }
        kernel_msg_free(msg);
    }


    // check if there is something in TX queue
    if(lecb->tx_sdu != NULL)
    {
        msg = kernel_param2msg(lecb->tx_sdu);

        // remove from TX queue if present
        common_list_extract(&(env->tx_queue), &(msg->hdr), 0);

        if(disconnect_ind)
        {
            // send disconnection indication
            struct l2cc_cmp_evt* cmp = KERNEL_MSG_ALLOC(L2CC_CMP_EVT,
                    msg->src_id, KERNEL_BUILD_ID(TASK_L2CC, conidx), l2cc_cmp_evt);
            cmp->operation = lecb->tx_sdu->operation;
            cmp->status    = GAP_ERR_DISCONNECTED;
            cmp->cid       = lecb->tx_sdu->sdu.cid;
            cmp->credit    = 0;
            kernel_msg_send(cmp);
        }

        kernel_msg_free(msg);
    }

    // remove allocated memory
    kernel_free(lecb);
}



void l2cc_lecb_init_disconnect(uint8_t conidx, struct l2cc_lecb_info* lecb, uint8_t disc_reason)
{
    // Create disconnect message
    struct l2cc_lecb_disconnect_cmd *req = KERNEL_MSG_ALLOC(L2CC_LECB_DISCONNECT_CMD,
            KERNEL_BUILD_ID(TASK_L2CC, conidx), KERNEL_BUILD_ID(TASK_L2CC, conidx),
            l2cc_lecb_disconnect_cmd);

    req->peer_cid  = lecb->peer_cid;
    req->operation = L2CC_LECB_DISCONNECT;

    kernel_msg_send(req);

    // put connection in disconnected state.
    SETB(lecb->state, L2CC_LECB_CONNECTED,   false);
    lecb->disc_reason  = disc_reason;
}

void l2cc_lecb_send_con_req(uint8_t conidx, uint8_t pkt_id, uint16_t le_psm, uint16_t scid, uint16_t credits,
                            uint16_t mps, uint16_t mtu)
{
    // send connection response
    struct l2cc_lecb_req *req = L2CC_SIG_PDU_ALLOC(conidx, L2C_CODE_LE_CB_CONN_REQ, KERNEL_BUILD_ID(TASK_L2CC, conidx), l2cc_lecb_req);

    // Set channel/packet ID and OP code
    req->pkt_id           = pkt_id;

    // fill up the parameters
    req->le_psm           = le_psm;
    req->scid             = scid;
    req->initial_credits  = credits;
    req->mps              = mps;
    req->mtu              = mtu;

    // send message
    l2cc_pdu_send(req);

    // start transaction timer
    kernel_timer_set(L2CC_SIGNALING_TRANS_TO_IND, KERNEL_BUILD_ID(TASK_L2CC, conidx), GAP_TMR_LECB_CONN_TIMEOUT);
}

void l2cc_lecb_send_con_rsp(uint8_t conidx, uint16_t status, uint8_t pkt_id,
                             uint16_t dcid, uint16_t credits, uint16_t mps, uint16_t mtu)
{
    // send connection response
    struct l2cc_lecb_rsp *rsp = L2CC_SIG_PDU_ALLOC(conidx, L2C_CODE_LE_CB_CONN_RESP, KERNEL_BUILD_ID(TASK_L2CC, conidx), l2cc_lecb_rsp);

    // Set channel/packet ID and OP code
    rsp->pkt_id           = pkt_id;

    // fill up the parameters
    rsp->dcid             = dcid;
    rsp->initial_credits  = credits;
    rsp->mps              = mps;
    rsp->mtu              = mtu;
    rsp->result           = status;

    // send message
    l2cc_pdu_send(rsp);
}


void l2cc_lecb_send_disc_req(uint8_t conidx, uint8_t pkt_id, uint16_t scid, uint16_t dcid)
{
    // send connection response
    struct l2cc_disconnection_req *req = L2CC_SIG_PDU_ALLOC(conidx, L2C_CODE_DISCONNECTION_REQ,
                                                    KERNEL_BUILD_ID(TASK_L2CC, conidx), l2cc_disconnection_req);

    // Set channel/packet ID and OP code
    req->pkt_id           = pkt_id;

    // fill up the parameters
    req->scid             = scid;
    req->dcid             = dcid;

    // send message
    l2cc_pdu_send(req);

    // start transaction timer
    kernel_timer_set(L2CC_SIGNALING_TRANS_TO_IND, KERNEL_BUILD_ID(TASK_L2CC, conidx), GAP_TMR_LECB_CONN_TIMEOUT);
}


void l2cc_lecb_send_disc_rsp(uint8_t conidx, uint8_t pkt_id, uint16_t dcid, uint16_t scid)
{
    // send connection response
    struct l2cc_disconnection_rsp *rsp = L2CC_SIG_PDU_ALLOC(conidx, L2C_CODE_DISCONNECTION_RESP,
                                                        KERNEL_BUILD_ID(TASK_L2CC, conidx), l2cc_disconnection_rsp);

    // Set channel/packet ID and OP code
    rsp->pkt_id = pkt_id;

    // fill up the parameters
    rsp->dcid   = dcid;
    rsp->scid   = scid;

    // send message
    l2cc_pdu_send(rsp);
}

void l2cc_lecb_send_credit_add(uint8_t conidx, uint8_t pkt_id, uint16_t cid, uint16_t credits)
{
    // send connection response
    struct l2cc_le_flow_ctl_credit *req = L2CC_SIG_PDU_ALLOC(conidx, L2C_CODE_LE_FLOW_CONTROL_CREDIT,
                                        KERNEL_BUILD_ID(TASK_L2CC, conidx), l2cc_le_flow_ctl_credit);

    // Set channel/packet ID and OP code
    req->pkt_id           = pkt_id;

    // fill up the parameters
    req->cid              = cid;
    req->credits          = credits;

    // send message
    l2cc_pdu_send(req);
}


#endif //(BLE_LECB)

/// @} L2CC_LECB
