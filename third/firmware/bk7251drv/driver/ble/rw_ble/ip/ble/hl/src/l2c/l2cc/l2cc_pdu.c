/**
 ****************************************************************************************
 *
 * @file l2cc_pdu.c
 *
 * @brief L2CAP Controller PDU packer / unpacker
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 *
 * Information about packet description
 *  - B: Byte value
 *  - W: Word value
 *  - L: Word value which contain length
 *  - A: 6 Bytes Bd Address value
 *  - K: 16 Bytes Key value
 *  - R: 8 Bytes random value
 *  - l[type]: Length information about following array of specified type
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @addtogroup L2CC_PDU
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"

#if (BLE_L2CC)

#include "l2cc_int.h"
#include "l2cc_pdu_int.h"
#include "l2cc_task.h"
#include "l2cc_lecb.h"
#include "l2cm_int.h" // Internal API required

#include "gattc.h"
#include "gattm.h"
#include "gapc.h"
#include "gapm.h"

#include "common_math.h"
#include "common_utils.h"
#include "common_list.h"
#include <string.h>


#ifdef BLE_AUDIO_AM0_TASK
#include "am0_api.h"
#endif // BLE_AUDIO_AM0_TASK


/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */


/// Signaling packet format
static const char* const l2cc_signaling_pkt_format[L2C_CODE_SIGNALING_MAX] =
{
    // Reserved code
    [L2C_CODE_RESERVED]                = NULL,
    // Reject request
    // [ Pkt_id | Len | reason | opt1 | opt2 ]
    [L2C_CODE_REJECT]                  = "BWWlW",
    // Connection request
    [L2C_CODE_CONNECTION_REQ]          = NULL,
    // Connection response
    [L2C_CODE_CONNECTION_RESP]         = NULL,
    // Configuration request
    [L2C_CODE_CONFIGURATION_REQ]       = NULL,
    // Configuration response
    [L2C_CODE_CONFIGURATION_RESP]      = NULL,
    // Disconnection request
    // [ Pkt_id | Len | Destination CID | Source CID ]
    [L2C_CODE_DISCONNECTION_REQ]       = "BLWW",
    // Disconnection response
    // [ Pkt_id | Len | Destination CID | Source CID ]
    [L2C_CODE_DISCONNECTION_RESP]      = "BLWW",
    // Echo request
    [L2C_CODE_ECHO_REQ]                = NULL,
    // Echo response
    [L2C_CODE_ECHO_RESP]               = NULL,
    // Information request
    [L2C_CODE_INFORMATION_REQ]         = NULL,
    // Information response
    [L2C_CODE_INFORMATION_RESP]        = NULL,
    // Create channel request
    [L2C_CODE_CREATE_CHANNEL_REQ]      = NULL,
    // Create channel response
    [L2C_CODE_CREATE_CHANNEL_RESP]     = NULL,
    // Move channel request
    [L2C_CODE_MOVE_CHANNEL_REQ]        = NULL,
    // Move channel response
    [L2C_CODE_MOVE_CHANNEL_RESP]       = NULL,
    // Move channel confirmation
    [L2C_CODE_MOVE_CHANNEL_CFM]        = NULL,
    // Move channel confirmation response
    [L2C_CODE_MOVE_CHANNEL_CFM_RESP]   = NULL,
    // Connection Parameter Update Request
    // [ Pkt_id | Len | ITV Min | ITV Max | Latency | Timeout ]
    [L2C_CODE_CONN_PARAM_UPD_REQ]      = "BLWWWW",
    // Connection Parameter Update Request
    // [ Pkt_id | Len | reason ]
    [L2C_CODE_CONN_PARAM_UPD_RESP]     = "BLW",
    // LE Credit Based Connection request
    // [ Pkt_id | Len | LE_PSM | Source CID | MTU | MPS | Initial Credits ]
    [L2C_CODE_LE_CB_CONN_REQ]          = "BLWWWWW",
    // LE Credit Based Connection response
    // [ Pkt_id | Len | Destination CID | MTU | MPS | Initial Credits | Result ]
    [L2C_CODE_LE_CB_CONN_RESP]         = "BLWWWWW",
    // LE Flow Control Credit
    // [ Pkt_id | Len | Destination CID | Credits ]
    [L2C_CODE_LE_FLOW_CONTROL_CREDIT]  = "BLWW",
};

/// Security packet format
static const char* const l2cc_security_pkt_format[L2C_CODE_SECURITY_MAX] =
{
    // Reserved code
    [L2C_CODE_RESERVED]                     = NULL,
    // Pairing Request
    // [iocap | oob | auth | max Key | IKeyX | RKeyX]
    [L2C_CODE_PAIRING_REQUEST]              = "BBBBBB",
    // Pairing Response
    // [iocap | oob | auth | max Key | IKeyX | RKeyX]
    [L2C_CODE_PAIRING_RESPONSE]             = "BBBBBB",
    // Pairing Confirm
    // [confirm]
    [L2C_CODE_PAIRING_CONFIRM]              = "K",
    // Pairing Random
    // [random]
    [L2C_CODE_PAIRING_RANDOM]               = "K",
    // Pairing Failed
    // [reason]
    [L2C_CODE_PAIRING_FAILED]               = "B",
    // Encryption Information
    // [LTK]
    [L2C_CODE_ENCRYPTION_INFORMATION]       = "K",
    // Master Identification
    // [ediv | rand]
    [L2C_CODE_MASTER_IDENTIFICATION]        = "WR",
    // Identity Information
    // [IRK]
    [L2C_CODE_IDENTITY_INFORMATION]         = "K",
    // Identity Address Information
    // [Addr_type | Addr]
    [L2C_CODE_IDENTITY_ADDRESS_INFORMATION] = "BA",
    // Signing Information
    // [CSRK]
    [L2C_CODE_SIGNING_INFORMATION]          = "K",
    // Security Request
    // [auth]
    [L2C_CODE_SECURITY_REQUEST]             = "B",

    #if (SECURE_CONNECTIONS)
    // Pairing Public Key
    // [public key X | public key Y]
    [L2C_CODE_PUBLIC_KEY]                   = "PP",

    // DHkey Check
    // [DHkey]
    [L2C_CODE_DHKEY_CHECK]                  = "K",

    // Key Press Notification
    // [Notification Type]
    [L2C_CODE_KEYPRESS_NOTIFICATION]        = "B"
    #endif // (SECURE_CONNECTIONS)
};



/// attribute protocol PDU packets format
static const char* const l2cc_attribute_pkt_format[L2C_CODE_ATT_MAX] =
{
    // Reserved code
    [L2C_CODE_RESERVED]                     = NULL,
    // Error response
    // [op_code | Handle | Err_Code]
    [L2C_CODE_ATT_ERR_RSP]                  = "BWB",
    // Exchange MTU Request
    // [MTU]
    [L2C_CODE_ATT_MTU_REQ]                  = "W",
    // Exchange MTU Response
    // [MTU]
    [L2C_CODE_ATT_MTU_RSP]                  = "W",
    // Find Information Request
    // [sHdl | eHdl]
    [L2C_CODE_ATT_FIND_INFO_REQ]            = "WW",
    // Find Information Response
    // [Format | Uuid]
    [L2C_CODE_ATT_FIND_INFO_RSP]            = "BlB",
    // Find By Type Value Request
    // [sHdl | eHdl | AttType | AttVal]
    [L2C_CODE_ATT_FIND_BY_TYPE_REQ]         = "WWWlB",
    // Find By Type Value Response
    // [InfoList]
    [L2C_CODE_ATT_FIND_BY_TYPE_RSP]         = "lB",
    // Read By Type Request
    // [sHdl | eHdl | UUID]
    [L2C_CODE_ATT_RD_BY_TYPE_REQ]           = "WWlB",
    // Read By Type Response
    // [number | data]
    [L2C_CODE_ATT_RD_BY_TYPE_RSP]           = "BlB",
    // Read Request
    // [Handle]
    [L2C_CODE_ATT_RD_REQ]                   = "W",
    // Read Response
    // [value]
    [L2C_CODE_ATT_RD_RSP]                   = "lB",
    // Read Blob Request
    // [Handle | Offset]
    [L2C_CODE_ATT_RD_BLOB_REQ]              = "WW",
    // Read Blob Response
    // [value]
    [L2C_CODE_ATT_RD_BLOB_RSP]              = "lB",
    // Read Multiple Request
    // [Handles]
    [L2C_CODE_ATT_RD_MULT_REQ]              = "lW",
    // Read Multiple Response
    // [value]
    [L2C_CODE_ATT_RD_MULT_RSP]              = "lB",
    // Read by Group Type Request
    // [sHdl | eHdl | UUID]
    [L2C_CODE_ATT_RD_BY_GRP_TYPE_REQ]       = "WWlB",
    // Read By Group Type Response
    // [number | data]
    [L2C_CODE_ATT_RD_BY_GRP_TYPE_RSP]       = "BlB",
    // Write Request
    // [Handle | Value]
    [L2C_CODE_ATT_WR_REQ]                   = "WlB",
    // Write Response
    // []
    [L2C_CODE_ATT_WR_RSP]                   = "",
    // Write Command
    // [Handle | Value]
    [L2C_CODE_ATT_WR_CMD_INFO]              = "WlB",
    // Signed Write Command
    // [Handle | Value]
    [L2C_CODE_ATT_SIGN_WR_CMD_INFO]         = "WlB",
    // Prepare Write Request
    // [Handle | Offset | Value]
    [L2C_CODE_ATT_PREP_WR_REQ]              = "WWlB",
    // Prepare Write Response
    // [Handle | Offset | Value]
    [L2C_CODE_ATT_PREP_WR_RSP]              = "WWlB",
    // Execute Write Request
    // [Flags]
    [L2C_CODE_ATT_EXE_WR_REQ]               = "B",
    // Execute Write Response
    // []
    [L2C_CODE_ATT_EXE_WR_RSP]               = "",
    // Handle Value Notification
    // [Handle | Value]
    [L2C_CODE_ATT_HDL_VAL_NTF]              = "WlB",
    // Handle Value Indication
    // [Handle | Value]
    [L2C_CODE_ATT_HDL_VAL_IND]              = "WlB",
    // Handle Value Confirmation
    // []
    [L2C_CODE_ATT_HDL_VAL_CFM]              = "",
};


/*
 * EXPORTED FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */

#if (BLE_LECB)

uint8_t l2cc_lecb_pdu_pack(uint8_t conidx, struct l2cc_sdu *sdu, uint16_t *length, uint8_t *buffer,
                           uint16_t* offset, uint16_t pdu_len, uint8_t llid)
{
    uint16_t cpy_length = 0;

    // New PDU, include header
    if (llid == L2C_PB_START_NON_FLUSH)
    {
        // First 2 bytes in L2Cap packet is packet length
        common_write16p(buffer, pdu_len);
        buffer     += L2C_LENGTH_LEN;
        cpy_length += L2C_LENGTH_LEN;

        // Next 2 bytes in L2Cap packet is channel ID
        common_write16p(buffer, sdu->cid);
        buffer     += L2C_CID_LEN;
        cpy_length += L2C_CID_LEN;

        // Put the SDU Length
        if(*offset == 0)
        {
            common_write16p(buffer, sdu->length);
            buffer      += L2C_SDU_LEN;
            cpy_length  += L2C_SDU_LEN;
            pdu_len     -= L2C_SDU_LEN;
        }
    }

    // Recalculate data length to copy
    *length = common_min(cpy_length + pdu_len, l2cm_get_buffer_size());
    cpy_length = *length - cpy_length;

    // Perform payload copy
    memcpy(buffer, &(sdu->data[*offset]), cpy_length);
    *offset     += cpy_length;

    return GAP_ERR_NO_ERROR;
}

uint8_t l2cc_lecb_pdu_unpack(struct l2cc_sdu *sdu, uint8_t *buffer, uint16_t length, uint16_t* offset,
                            uint16_t* pdu_remain_len, uint8_t llid)
{
    // Status of unpacking
    uint8_t status = GAP_ERR_NO_ERROR;

    // Remove PDU packet header
    if(llid != L2C_PB_CONTINUE)
    {
        buffer     += L2C_LENGTH_LEN + L2C_CID_LEN;
        length     -= L2C_LENGTH_LEN + L2C_CID_LEN;
    }

    // remove SDU length info from PDU, already retrieved
    if(*offset == 0)
    {
        buffer          += L2C_SDU_LEN;
        length          -= L2C_SDU_LEN;
        *pdu_remain_len -= L2C_SDU_LEN;
    }

    // Check that segment size not exceeded by received fragment
    if(length > *pdu_remain_len)
    {
        status = L2C_ERR_INVALID_PDU;
    }
    else
    {
        // Copy data fragment information
        memcpy(&(sdu->data[*offset]), buffer, length);

        // update internal cursors
        *offset         += length;
        *pdu_remain_len -= length;
    }

    return (status);
}

#endif //(BLE_LECB)


#if (BLE_DEBUG)

uint8_t l2cc_dbg_pdu_pack(struct l2cc_dbg_pdu *pdu, uint16_t *length, uint8_t *buffer, uint16_t* offset, uint8_t *llid)
{
    uint8_t status = GAP_ERR_NO_ERROR;
    *length    = common_min(l2cm_get_buffer_size(), pdu->length);

    if(*offset == 0)
    {
        *llid = L2C_PB_START_NON_FLUSH;
    }
    else
    {
        *llid = L2C_PB_CONTINUE;
    }

    // Perform payload copy
    memcpy(buffer, &(pdu->data[*offset]), *length);
    *offset     += *length;
    pdu->length -= *length;

    return status;
}

uint8_t l2cc_dbg_pdu_unpack(struct l2cc_dbg_pdu *pdu, uint8_t *buffer, uint16_t length, uint16_t* offset, uint8_t llid)
{
    // Status of unpacking
    uint8_t status = GAP_ERR_NO_ERROR;

    // check that packet size not exceed
    if(length > (pdu->length - *offset))
    {
        status = L2C_ERR_INVALID_PDU;
    }
    else
    {
        // Copy data fragment information
        memcpy(pdu->data + *offset, buffer, length);

        // update internal cursors
        *offset += length;
    }
    return (status);
}
#endif //  (BLE_DEBUG)



uint8_t l2cc_pdu_pack(struct l2cc_pdu *p_pdu, uint16_t *p_offset, uint16_t *p_length, uint8_t *p_buffer, uint8_t *llid)
{
    // Indicate if the packet is well formated
    uint8_t status = GAP_ERR_NO_ERROR;

    // Initialize the length of the packet
    *p_length = 0;

    // Check the current state of the fragmentation - If p_data_pack is NULL, first packet
    if (*llid == L2C_PB_START_NON_FLUSH)
    {
        // Packet descriptor information
        uint8_t pkt_desc_array_size = 0;
        uint8_t **pkt_desc_array = NULL;
        uint8_t *pkt_desc = NULL;
        // Code request
        uint8_t code = p_pdu->data.code;
        // Current position in the buffer which contains the packet to be sent
        uint8_t *output = p_buffer + L2C_HEADER_LEN;
        // Set the data pointer
        uint8_t *p_data_pack  = &(p_pdu->data.code) + L2C_CODE_LEN;

        // Initialize the total payload length
        p_pdu->payld_len = 0;

        do
        {
            // Extract channel identifier
            switch (p_pdu->chan_id)
            {
                // Signaling packet
                case (L2C_CID_LE_SIGNALING):
                {
                    pkt_desc_array = (uint8_t **)l2cc_signaling_pkt_format;
                    pkt_desc_array_size = L2C_CODE_SIGNALING_MAX;
                } break;

                case (L2C_CID_SECURITY):
                {
                    pkt_desc_array = (uint8_t **)l2cc_security_pkt_format;
                    pkt_desc_array_size = L2C_CODE_SECURITY_MAX;
                } break;

                case (L2C_CID_ATTRIBUTE):
                {
                    // Update code to an existing index in packet definition array
                    if (code == L2C_CODE_ATT_WR_CMD)
                    {
                        code = L2C_CODE_ATT_WR_CMD_INFO;
                    }
                    else if (code == L2C_CODE_ATT_SIGN_WR_CMD)
                    {
                        code = L2C_CODE_ATT_SIGN_WR_CMD_INFO;
                    }

                    pkt_desc_array = (uint8_t **)l2cc_attribute_pkt_format;
                    pkt_desc_array_size = L2C_CODE_ATT_MAX;
                } break;

                #ifdef BLE_AUDIO_AM0_TASK
                // Audio Mode 0 L2CAP Protocol
                case (AM0_L2C_CID_AUDIO_MODE_0):
                {
                    if(gapm_is_audio_am0_sup())
                    {
                        pkt_desc_array = am0_pdu_pkt_format_get(&pkt_desc_array_size);
                        break;
                    }
                } // no break
                #endif // BLE_AUDIO_AM0_TASK

                default:
                {
                    // Error code, invalid CID.
                    status = L2C_ERR_INVALID_CID;
                } break;
            }

            // Stop if an error occurs
            if (status != GAP_ERR_NO_ERROR)
            {
                break;
            }

            // Check if code is valid or not
            if ((pkt_desc_array == NULL) || (code >= pkt_desc_array_size))
            {
                // Error packet code invalid
                status = L2C_ERR_INVALID_PDU;
                break;
            }

            pkt_desc = pkt_desc_array[code];

            // Check if it's a known packet type
            if (pkt_desc == NULL)
            {
                // error packet invalid cannot be extracted
                status = L2C_ERR_INVALID_PDU;
                break;
            }

            // Pointer used to update the length value in a signaling packet
            uint8_t* len_ptr = NULL;

            // Set PDU code
            *output = p_pdu->data.code;
            output += L2C_CODE_LEN;
            p_pdu->payld_len += L2C_CODE_LEN;
            *p_length += L2C_CODE_LEN;

            // Parse packet description
            while ((*pkt_desc != '\0') && (status == GAP_ERR_NO_ERROR))
            {
                // Check the packet descriptor character by character
                switch (*pkt_desc)
                {
                    case ('B'): // Byte
                    {
                        *output = *p_data_pack;

                        output += 1;
                        p_data_pack += 1;
                        p_pdu->payld_len += 1;
                        *p_length += 1;
                    } break;

                    case ('W'): // Word
                    {
                        // Align data buffer to a 16 bit
                        uint16_t *word = ((uint16_t *)((((uint32_t)p_data_pack) + 1) & ~1));

                        p_data_pack = (uint8_t *)(word + 1);
                        p_pdu->payld_len += 2;
                        *p_length += 2;

                        common_write16p(output, *word);
                        output += 2;
                    } break;

                    case ('L'): // Word Length
                    {
                        // Align data buffer to a 16 bit
                        uint16_t *word = ((uint16_t *)((((uint32_t)p_data_pack) + 1) & ~1));

                        p_data_pack = (uint8_t *)(word + 1);
                        len_ptr = output;

                        p_pdu->payld_len += 2;
                        *p_length += 2;
                        output += 2;
                    } break;

                    case ('K'): // Key
                    {
                        // Pack the key value
                        memcpy(output, p_data_pack, GAP_KEY_LEN);

                        p_data_pack += GAP_KEY_LEN;
                        output += GAP_KEY_LEN;
                        p_pdu->payld_len += GAP_KEY_LEN;
                        *p_length += GAP_KEY_LEN;
                    } break;

                    case ('R'): // Random Value
                    {
                        // Pack the rand value
                        memcpy(output, p_data_pack, GAP_RAND_NB_LEN);

                        p_data_pack += GAP_RAND_NB_LEN;
                        output += GAP_RAND_NB_LEN;
                        p_pdu->payld_len += GAP_RAND_NB_LEN;
                        *p_length += GAP_RAND_NB_LEN;
                    } break;

                    case ('A'): // BD Address
                    {
                        // Pack the bd address value
                        memcpy(output, p_data_pack, BD_ADDR_LEN);

                        p_data_pack += BD_ADDR_LEN;
                        output += BD_ADDR_LEN;
                        p_pdu->payld_len += BD_ADDR_LEN;
                        *p_length += BD_ADDR_LEN;
                    } break;

                    // Array of bytes or words to pack
                    case ('l'):
                    {
                        // Length of data that can be inserted in the packet
                        uint16_t avail_length = l2cm_get_buffer_size() - *p_length - L2C_HEADER_LEN;
                        // Remaining length of payload
                        uint16_t rem_len;

                        // Get the type of unit
                        pkt_desc++;

                        // Align data pointer to a 16 bits
                        p_data_pack = (uint8_t *)((((uint32_t)p_data_pack) + 1) & ~1);

                        // Set the remaining length
                        rem_len = common_read16p(p_data_pack);

                        // Update pointer of data value - uint16_t
                        p_data_pack += 2;

                        // Check array type
                        if (*pkt_desc == 'W')
                        {
                            // Update remaining length value
                            rem_len *= 2;
                        }
                        else if (*pkt_desc != 'B')
                        {
                            // error, invalid PDU
                            status = L2C_ERR_INVALID_PDU;
                            break;
                        }

                        // Update the total length of payload
                        p_pdu->payld_len += rem_len;

                        // Check if the remaining data can be inserted in the packet
                        avail_length = common_min(avail_length, rem_len);

                        // Copy the payload in the buffer
                        memcpy(output, p_data_pack, avail_length);

                        // Update the packet length
                        *p_length += avail_length;
                        // Update the data pointer
                        p_data_pack += avail_length;
                    } break;

                    #if (SECURE_CONNECTIONS)
                    // 32 Byte public Key Co-ordinate
                    case ('P'):
                    {
                        memcpy(output, p_data_pack, GAP_P256_KEY_LEN);
                        p_data_pack += GAP_P256_KEY_LEN;
                        output += GAP_P256_KEY_LEN;
                        p_pdu->payld_len += GAP_P256_KEY_LEN;
                        *p_length += GAP_P256_KEY_LEN;
                    }
                    break;
                    #endif // (SECURE_CONNECTIONS)
                    default:
                    {
                        // packet description error
                        status = L2C_ERR_INVALID_PDU;
                    } break;
                }

                pkt_desc++;
            }

            // Stop if an error occurs
            if (status != GAP_ERR_NO_ERROR)
            {
                break;
            }

            // If a 'L' characteristic has been found (Length Word), set the length
            if (len_ptr != NULL)
            {
                // Length is packet length - L2CAP Header - Code Length - Pkt Id - Length param length
                common_write16p(len_ptr, (uint16_t)(*p_length - L2C_CODE_LEN - 1 - 2));
            }
        } while(0);


        // If no error detected
        if (status == GAP_ERR_NO_ERROR)
        {
            // Check payload length
            if (p_pdu->payld_len > gapm_get_max_mtu())
            {
                status = L2C_ERR_INVALID_MTU_EXCEED;
            }
            else
            {
                // First 2 bytes in L2Cap packet is packet length
                common_write16p(p_buffer, p_pdu->payld_len);
                p_buffer += L2C_LENGTH_LEN;

                // Next 2 bytes in L2Cap packet is channel ID
                common_write16p(p_buffer, p_pdu->chan_id);
                *p_length += L2C_HEADER_LEN;

                // Decrease the remaining size of data to pack
                p_pdu->payld_len -= (*p_length - L2C_HEADER_LEN);
            }

            // Update the offset value for fragmentation
            *p_offset = (uint16_t)(p_data_pack - &(p_pdu->data.code));
        }
    }
    else
    {
        // Length of data that can be inserted in the packet
        uint16_t cpy_length = common_min(l2cm_get_buffer_size(), p_pdu->payld_len);
        // Copied payload length
        uint16_t pyld_len = cpy_length;
        // Current position in the buffer which contains the packet to be sent
        uint8_t *output = p_buffer;
        // Current position in the PDU which contains the packet to be sent
        uint8_t *input = &(p_pdu->data.code);

        // Copy the payload in the buffer
        memcpy(output, (input + *p_offset), pyld_len);

        // Update the packet length
        *p_length += cpy_length;
        // Update the offset value for fragmentation
        *p_offset += pyld_len;

        // Decrease the remaining size of data to pack
        p_pdu->payld_len -= pyld_len;
    }

    return (status);
}

uint8_t l2cc_pdu_unpack(struct l2cc_pdu *p_pdu, uint16_t *p_offset, uint16_t *p_rem_len,
                        const uint8_t *p_buffer, uint16_t pkt_length, uint8_t llid)
{
    // Status of unpacking
    uint8_t status = GAP_ERR_NO_ERROR;

    if (llid == L2C_PB_START_FLUSH)
    {
        uint8_t code = 0;
        uint8_t total_length = 0;

        // Packet descriptor information
        uint8_t **pkt_desc_array = NULL;
        uint8_t pkt_desc_array_size = L2C_CODE_RESERVED;
        uint8_t *pkt_desc = NULL;

        uint8_t *input = (uint8_t *)p_buffer;

        // Pointer to the data to pack
        uint8_t *p_data_pack;
        // packet can be divided in multiple fragment
        bool mult_fragment = false;

        // First 2 bytes in L2Cap packet is packet length
        p_pdu->payld_len = common_read16p(input);
        input += L2C_LENGTH_LEN;
        total_length += 2;

        // Next 2 bytes in L2Cap packet is channel ID
        p_pdu->chan_id = common_read16p(input);
        input += L2C_CID_LEN;
        total_length += 2;

        // Next byte is the operation code
        code = *input;

        do
        {
            // Extract channel identifier
            switch (p_pdu->chan_id)
            {
                // signaling packet
                case (L2C_CID_LE_SIGNALING):
                {
                    // identifier 0 is invalid
                    if (*(input + L2C_CODE_LEN))
                    {
                        pkt_desc_array = (uint8_t **)l2cc_signaling_pkt_format;
                        pkt_desc_array_size = L2C_CODE_SIGNALING_MAX;
                    }
                } break;

                case (L2C_CID_SECURITY):
                {
                    pkt_desc_array = (uint8_t **)l2cc_security_pkt_format;
                    pkt_desc_array_size = L2C_CODE_SECURITY_MAX;
                } break;

                case (L2C_CID_ATTRIBUTE):
                {
                    // Update code to an existing index in packet definition array
                    if (code == L2C_CODE_ATT_WR_CMD)
                    {
                        code = L2C_CODE_ATT_WR_CMD_INFO;
                    }
                    else if (code == L2C_CODE_ATT_SIGN_WR_CMD)
                    {
                        code = L2C_CODE_ATT_SIGN_WR_CMD_INFO;
                    }

                    pkt_desc_array = (uint8_t **)l2cc_attribute_pkt_format;
                    pkt_desc_array_size = L2C_CODE_ATT_MAX;
                } break;

                #ifdef BLE_AUDIO_AM0_TASK
                // Audio Mode 0 L2CAP Protocol
                case (AM0_L2C_CID_AUDIO_MODE_0):
                {
                    if(gapm_is_audio_am0_sup())
                    {
                        pkt_desc_array = am0_pdu_pkt_format_get(&pkt_desc_array_size);

                        break;
                    }
                } // no break
                #endif // BLE_AUDIO_AM0_TASK

                default:
                {
                    // Shall not happen
                    ASSERT_INFO(0, p_pdu->chan_id, p_pdu->payld_len);

                    // Error code, Invalid CID.
                    status = L2C_ERR_INVALID_CID;
                } break;
            }

            // Error occurs stop parsing
            if (status != GAP_ERR_NO_ERROR)
            {
                break;
            }

            // check if code is valid or not
            if((pkt_desc_array == NULL) || (code >= pkt_desc_array_size))
            {
                // error packet code invalid
                status = L2C_ERR_INVALID_PDU;
                break;
            }

            pkt_desc = pkt_desc_array[code];

            // check if it's a known packet type
            if (pkt_desc == NULL)
            {
                // error packet cannot be extracted, not supported
                status = L2C_ERR_INVALID_PDU;
                break;
            }

            // Extract pdu code
            p_pdu->data.code = *input;
            input += L2C_CODE_LEN;
            total_length += L2C_CODE_LEN;

            p_data_pack = &(p_pdu->data.code) + L2C_CODE_LEN;

            // parse packet description
            while ((*pkt_desc != '\0') && (status == GAP_ERR_NO_ERROR) && (total_length <= *p_rem_len))
            {
                // check character by character
                switch (*pkt_desc)
                {
                    case ('B'): // Byte
                    {
                        *p_data_pack = *input;

                        input += 1;
                        p_data_pack  += 1;
                        total_length += 1;
                    } break;

                    case ('L'): // Word Length
                    case ('W'): // Word
                    {
                        // Align data buffer to a 16 bit
                        uint16_t *word = ((uint16_t *)((((uint32_t)p_data_pack) + 1) & ~1));

                        p_data_pack = (uint8_t *)(word + 1);

                        *word = common_read16p(input);
                        input += 2;
                        total_length += 2;
                    } break;

                    case ('K'):
                    {
                        // Unpack the key value
                        memcpy(p_data_pack, input, GAP_KEY_LEN);

                        p_data_pack += GAP_KEY_LEN;
                        input += GAP_KEY_LEN;
                        total_length += GAP_KEY_LEN;
                    } break;
                    #if (SECURE_CONNECTIONS)
                    case ('P'):
                    {
                         // Remaining data length in the received packet
                         uint8_t len = common_min((pkt_length - total_length), GAP_P256_KEY_LEN);

                         // Unpack the key value
                         memcpy(p_data_pack, input, len);

                         if(len != GAP_P256_KEY_LEN)
                         {
                             mult_fragment = true;
                         }

                         p_data_pack += len;
                         input += len;
                         total_length += len;

                     } break;
                    #endif //  (SECURE_CONNECTIONS)

                    case ('R'):
                    {
                        // Unpack the rand value
                        memcpy(p_data_pack, input, GAP_RAND_NB_LEN);

                        p_data_pack += GAP_RAND_NB_LEN;
                        input += GAP_RAND_NB_LEN;
                        total_length += GAP_RAND_NB_LEN;
                    } break;

                    case ('A'):
                    {
                        // Unpack the bd address value
                        memcpy(p_data_pack, input, BD_ADDR_LEN);

                        p_data_pack += BD_ADDR_LEN;
                        input += BD_ADDR_LEN;
                        total_length += BD_ADDR_LEN;
                    } break;

                    // Array of byte or word to unpack
                    case ('l'):
                    {
                        // Remaining data length in the received packet
                        uint8_t len = pkt_length - total_length;
                        uint16_t length = p_pdu->payld_len - (total_length - L2C_HEADER_LEN);
                        mult_fragment = true;

                        pkt_desc++;
                        // Align data pointer to a 16 bits
                        p_data_pack = (uint8_t *)((((uint32_t)p_data_pack) + 1) & ~1);

                        // Check array type
                        if (*pkt_desc == 'W')
                        {
                            // Divide number of data in the array by 2
                            length /= 2;
                        }
                        else if (*pkt_desc != 'B')
                        {
                            // Error, invalid PDU
                            status = L2C_ERR_INVALID_PDU;
                            break;
                        }

                        // Number of data in the array = Total remaining payload size
                        common_write16p(p_data_pack, length);

                        // Update pointer of data value - uint16_t
                        p_data_pack += 2;

                        // Retrieve data
                        memcpy(p_data_pack, input, len);

                        total_length += len;
                        p_pdu->payld_len += 2;
                        p_data_pack  += len;
                    } break;

                    default:
                    {
                        // Packet description error
                        status = L2C_ERR_INVALID_PDU;
                    } break;
                }

                pkt_desc++;
            }

            // check that received PDU length is not less than expected PDU
            if(((uint16_t)total_length > *p_rem_len)
                    // Packet not fully extracted
                    || (*pkt_desc != '\0')
                    // Or received data is bigger than expected (does not contains an array of byte)
                    || (!mult_fragment && ((uint16_t)total_length != *p_rem_len)))
            {
                // check if SIG MTU exceed
                if((p_pdu->chan_id == L2C_CID_LE_SIGNALING) && (p_pdu->payld_len > L2C_MIN_LE_MTUSIG))
                {
                    status = L2C_ERR_INVALID_MTU_EXCEED;
                }
                else
                {
                    status = L2C_ERR_INVALID_PDU;
                }
            }
            else
            {
                // Update the remaining length
                *p_rem_len -= (uint16_t)total_length;
                // Update the offset value
                *p_offset += (uint16_t)(p_data_pack - &(p_pdu->data.code));
            }
        } while (0);

        if (status != GAP_ERR_NO_ERROR)
        {
            switch (p_pdu->chan_id)
            {
                case (L2C_CID_ATTRIBUTE):
                case (L2C_CID_SECURITY):
                {
                    // Nothing to do
                } break;

                // Signaling packet
                default:
                {
                    // Retrieve packet id
                    p_pdu->data.reject.pkt_id = *(p_buffer + L2C_LENGTH_LEN + L2C_CID_LEN + L2C_CODE_LEN);
                } break;
            }
        }
    }
    else
    {
        if (pkt_length <= *p_rem_len)
        {
            // Retrieve data
            memcpy(&p_pdu->data.code + *p_offset, p_buffer, pkt_length);

            *p_offset += pkt_length;
            // Update the remaining length
            *p_rem_len -= pkt_length;
        }
        else
        {
            // Length of the PDU is not valid
            status = L2C_ERR_INVALID_PDU;
        }
    }

    return (status);
}


uint8_t l2cc_pdu_header_check(uint8_t conidx, uint8_t* buffer)
{
    struct l2cc_env_tag* env = l2cc_env[conidx];

    // segment length + CID
    uint16_t seg_length;
    uint16_t cid;
    uint8_t status       = GAP_ERR_NO_ERROR;

    // If the packet is not a new part of the payload, drop the current PDU
    if (env->rx_buffer)
    {
        #if (BLE_LECB)
        if(env->rx_buffer->id == L2CC_LECB_SDU_RECV_IND)
        {
            struct l2cc_lecb_sdu_recv_ind* sdu = (struct l2cc_lecb_sdu_recv_ind*) kernel_msg2param(env->rx_buffer);
            struct l2cc_lecb_info* lecb = l2cc_lecb_find(conidx, L2CC_LECB_LOCAL_CID, sdu->sdu.cid);
            if(lecb != NULL)
            {
                lecb->rx_sdu = NULL;

                l2cc_lecb_init_disconnect(conidx, lecb, L2C_ERR_INVALID_PDU);
            }
            kernel_msg_free(env->rx_buffer);
        }
        else
        #endif // (BLE_LECB)
        {
            // all indication message have status indication in first byte parameter
            uint8_t* ind_status = (uint8_t*) kernel_msg2param(env->rx_buffer);
            *ind_status = L2C_ERR_INVALID_PDU;
            kernel_msg_send(ind_status);
        }
        env->rx_buffer = NULL;
    }

    // Read header packet information
    seg_length  = common_read16p(buffer);
    cid         = common_read16p(buffer+L2C_LENGTH_LEN);

    #if (BLE_DEBUG)
    // Debug feature, Forward messages to the APP except ATT, Security and some LE_SIG messages
    if (gapm_dbg_mode_en())
    {
        status   = GAP_ERR_NO_ERROR;

        // check default MTU
        if(seg_length > gapm_get_max_mtu())
        {
            status = L2C_ERR_INVALID_MTU_EXCEED;
        }

        switch(cid)
        {
            case L2C_CID_SECURITY:  /* SMP block */
            case L2C_CID_ATTRIBUTE: /* ATT block */
            #ifdef BLE_AUDIO_AM0_TASK
                // Audio Mode 0 L2CAP Protocol
            case (AM0_L2C_CID_AUDIO_MODE_0):
            #endif // BLE_AUDIO_AM0_TASK
            {
                /* nothing to do */
            } break;


            case L2C_CID_LE_SIGNALING: /* GAP block */
            {
                uint8_t code = *(buffer+L2C_LENGTH_LEN+L2C_CID_LEN);

                // ignore parameter update codes
                if((code == L2C_CODE_CONN_PARAM_UPD_REQ) || (code == L2C_CODE_CONN_PARAM_UPD_RESP))
                {
                    break;
                }
            }
            // no break

            default:
            {
                // create a debug message that will be handled by application
                struct l2cc_dbg_pdu_recv_ind* dbg_pdu;
                dbg_pdu = KERNEL_MSG_ALLOC_DYN(L2CC_DBG_PDU_RECV_IND, APP_MAIN_TASK, KERNEL_BUILD_ID(TASK_L2CC, conidx),
                        l2cc_dbg_pdu_recv_ind, (status == GAP_ERR_NO_ERROR) ? seg_length + L2C_HEADER_LEN : 0);

                dbg_pdu->offset     = 0;
                dbg_pdu->status     = status;
                dbg_pdu->pdu.length = (status == GAP_ERR_NO_ERROR) ? seg_length + L2C_HEADER_LEN : 0;


                env->rx_buffer = kernel_param2msg(dbg_pdu);
            } break;
        }
    }
    #endif // (BLE_DEBUG)


    if(env->rx_buffer == NULL)
    {
        // Handle new PDU
        switch(cid)
        {
            case L2C_CID_SECURITY:  /* SMP block */
            case L2C_CID_LE_SIGNALING: /* GAP block */
            case L2C_CID_ATTRIBUTE: /* ATT block */
            #ifdef BLE_AUDIO_AM0_TASK
            // Audio Mode 0 L2CAP Protocol
            case (AM0_L2C_CID_AUDIO_MODE_0):
            #endif // BLE_AUDIO_AM0_TASK
            {
                kernel_task_id_t dest_id  = KERNEL_BUILD_ID(TASK_GAPC, conidx);

                // check default MTU
                if(seg_length > gapm_get_max_mtu())
                {
                    status = L2C_ERR_INVALID_MTU_EXCEED;
                    break;
                }

                if(cid == L2C_CID_ATTRIBUTE)
                {
                    dest_id = KERNEL_BUILD_ID(TASK_GATTC, conidx);
                    // Check GATT specific MTU
                    if(seg_length > gattc_get_mtu(conidx))
                    {
                        status = L2C_ERR_INVALID_MTU_EXCEED;
                        break;
                    }
                }
                else if(cid == L2C_CID_LE_SIGNALING)
                {
                    dest_id = KERNEL_BUILD_ID(TASK_L2CC, conidx);
                }
                else if(cid == L2C_CID_LE_SIGNALING)
                {
                    dest_id = KERNEL_BUILD_ID(TASK_L2CC, conidx);
                }
                #ifdef BLE_AUDIO_AM0_TASK
                else if(cid == AM0_L2C_CID_AUDIO_MODE_0)
                {
                    dest_id = KERNEL_BUILD_ID(TASK_AM0, conidx);
                }
                #endif // BLE_AUDIO_AM0_TASK

                // create a message that will be handled by upper layers
                struct l2cc_pdu_recv_ind* pdu;
                pdu = KERNEL_MSG_ALLOC_DYN(L2CC_PDU_RECV_IND, dest_id, KERNEL_BUILD_ID(TASK_L2CC, conidx),
                        l2cc_pdu_recv_ind, ((status == GAP_ERR_NO_ERROR) ? seg_length : 0));

                pdu->offset        = 0;
                pdu->status        = status;
                pdu->pdu.chan_id   = cid;
                pdu->pdu.payld_len = (status == GAP_ERR_NO_ERROR) ? seg_length : 0;

                env->rx_buffer = kernel_param2msg(pdu);

                // store size of segment expected
                env->rx_pdu_rem_len = seg_length + L2C_HEADER_LEN;

            }
            break;

            default:
            {
                status = L2C_ERR_INVALID_CID;

                #if (BLE_LECB)
                /* GAP block for LE Credit Based channels*/
                if ((status == L2C_ERR_INVALID_CID) && L2C_IS_DYNAMIC_CID(cid))
                {
                    // 1. Find if it's reception of a first segment.
                    struct l2cc_lecb_info* lecb = l2cc_lecb_find(conidx, L2CC_LECB_LOCAL_CID, cid);
                    struct l2cc_lecb_sdu_recv_ind* sdu = NULL;

                    if((lecb != NULL) && GETB(lecb->state, L2CC_LECB_CONNECTED))
                    {
                        uint16_t sdu_length;
                        uint16_t offset = 0;

                        status = GAP_ERR_NO_ERROR;

                        // Sdu not fully received
                        if(lecb->rx_sdu != NULL)
                        {
                            env->rx_buffer = kernel_param2msg(lecb->rx_sdu);
                            sdu            = lecb->rx_sdu;
                            offset         = sdu->offset;
                            sdu_length     = sdu->sdu.length;
                        }
                        else
                        {
                            sdu_length = common_read16p(buffer + L2C_LENGTH_LEN + L2C_CID_LEN);
                        }

                        // 1. Check MPS Size
                        if(seg_length > lecb->local_mps)
                        {
                            status       = L2C_ERR_INVALID_MPS_EXCEED;
                        }
                        // 2. Check number of credit
                        else if(lecb->local_credit == 0)
                        {
                            status       = L2C_ERR_INSUFF_CREDIT;
                        }
                        // 3. Check MTU Size
                        else if(sdu_length > lecb->local_mtu)
                        {
                            status       = L2C_ERR_INVALID_MTU_EXCEED;
                        }
                        // 4. check if previous PDU was ok and if new segment does not exceed SDU length
                        else if(   ((offset == 0) && (offset + seg_length) > (sdu_length + L2C_SDU_LEN))
                                || ((offset != 0) && (offset + seg_length) > (sdu_length)))
                        {
                            status       = L2C_ERR_INVALID_PDU;
                        }

                        if(status == GAP_ERR_NO_ERROR)
                        {
                            uint16_t nb_credit_expect;

                            // new segment, decrease number of available credit
                            lecb->local_credit--;

                            // 5. create SDU if does not exist
                            if(sdu == NULL)
                            {
                                sdu = KERNEL_MSG_ALLOC_DYN(L2CC_LECB_SDU_RECV_IND, lecb->task_id, KERNEL_BUILD_ID(TASK_L2CC, conidx),
                                        l2cc_lecb_sdu_recv_ind, sdu_length);

                                // Initialize it.
                                sdu->sdu.credit = 1;
                                sdu->sdu.cid    = cid;
                                sdu->sdu.length = sdu_length;

                                // Indicates the position of buffer to be updated
                                sdu->offset    = 0;

                                lecb->rx_sdu        = sdu;
                                env->rx_buffer = kernel_param2msg(lecb->rx_sdu);

                                // update PDU status
                                sdu->status = GAP_ERR_NO_ERROR;
                            }
                            else
                            {
                                // increment number of used credits
                                sdu->sdu.credit++;

                                // 6. check if a credit can be automatically added
                                nb_credit_expect = ((offset + L2C_SDU_LEN + seg_length + (lecb->local_mps-1)) / lecb->local_mps);

                                if(nb_credit_expect < sdu->sdu.credit)
                                {
                                    struct l2cc_lecb_add_cmd* add = KERNEL_MSG_ALLOC(L2CC_LECB_ADD_CMD, KERNEL_BUILD_ID(TASK_L2CC, conidx),
                                            KERNEL_BUILD_ID(TASK_L2CC, conidx), l2cc_lecb_add_cmd);

                                    add->operation = L2CC_LECB_CREDIT_ADD;
                                    add->local_cid = cid;
                                    add->credit    = 1;
                                    sdu->sdu.credit--;

                                    kernel_msg_send(add);
                                }
                            }

                            // store size of segment expected
                            env->rx_pdu_rem_len = seg_length;
                        }
                        else
                        {
                            l2cc_lecb_init_disconnect(conidx, lecb, status);

                            if(env->rx_buffer)
                            {
                                // drop the packet
                                kernel_msg_free(env->rx_buffer);
                                env->rx_buffer = NULL;
                            }
                            lecb->rx_sdu   = NULL;
                        }
                    }
                }
                #endif // (BLE_LECB)

                if (status == L2C_ERR_INVALID_CID)
                {
                    // invalid CID, handle it in l2cap to send reject
                    struct l2cc_pdu_recv_ind* pdu;
                    pdu = KERNEL_MSG_ALLOC_DYN(L2CC_PDU_RECV_IND, KERNEL_BUILD_ID(TASK_L2CC, conidx), KERNEL_BUILD_ID(TASK_L2CC, conidx),
                            l2cc_pdu_recv_ind, 0);

                    pdu->offset        = 0;
                    pdu->status        = status;
                    pdu->pdu.chan_id   = cid;
                    pdu->pdu.payld_len = 0;

                    env->rx_buffer = kernel_param2msg(pdu);
                }
            } break;
        }
    }

    // an error occurs, inform upper layers about the RX issue
    if((env->rx_buffer != NULL) && (status != GAP_ERR_NO_ERROR))
    {
        kernel_msg_send(kernel_msg2param(env->rx_buffer));
        env->rx_buffer = NULL;
    }


    return status;
}


#endif //(BLE_L2CC)

/// @} L2CC_PDU
