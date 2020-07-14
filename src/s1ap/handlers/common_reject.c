/*
* Copyright 2019-present Open Networking Foundation
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include "log.h"
#include "err_codes.h"
#include "s1ap.h"
#include "ipc_api.h"
#include "main.h"
#include "sctp_conn.h"
#include "msgType.h"


/**
* Get ProtocolIE value for Service Reject sent by mme-app
*/
static int
get_serviceReject_protoie_value(struct proto_IE *value, struct commonRej_info *g_mmeS1apInfo)
{
    value->no_of_IEs = SERVICE_REJECT_NO_OF_IES;

    value->data = (proto_IEs *) malloc(SERVICE_REJECT_NO_OF_IES*
            sizeof(proto_IEs));

    value->data[0].val.mme_ue_s1ap_id = g_mmeS1apInfo->ue_idx;
    value->data[1].val.enb_ue_s1ap_id = g_mmeS1apInfo->s1ap_enb_ue_id;

    log_msg(LOG_INFO, "mme_ue_s1ap_id %d and enb_ue_s1ap_id %d\n",
		g_mmeS1apInfo->ue_idx, g_mmeS1apInfo->s1ap_enb_ue_id);

    return SUCCESS;
}


/**
* Stage specific message processing.
*/
static int
s1ap_service_reject_processing(struct commonRej_info *g_mmeS1apInfo)
{
    struct s1ap_PDU s1apPDU = {0};
    struct Buffer g_buffer = {0};
    struct Buffer g_value_buffer = {0};

    /* Assigning values to s1apPDU */
    s1apPDU.procedurecode = id_downlinkNASTransport;
    s1apPDU.criticality = CRITICALITY_IGNORE;

    get_serviceReject_protoie_value(&s1apPDU.value, g_mmeS1apInfo);

    /* Copy values to buffer from s1apPDU */

    g_buffer.pos = 0;

    uint8_t initiating_message = 0; /* TODO: Add enum */
    buffer_copy(&g_buffer, &initiating_message,
            sizeof(initiating_message));

    buffer_copy(&g_buffer, &s1apPDU.procedurecode,
            sizeof(s1apPDU.procedurecode));

    buffer_copy(&g_buffer, &s1apPDU.criticality,
            sizeof(s1apPDU.criticality));

    /* Copy values in g_value_buffer */
    g_value_buffer.pos = 0;

    /* TODO remove hardcoded values */
    unsigned char chProtoIENo[3] = {0,0,3};

    buffer_copy(&g_value_buffer, chProtoIENo, 3);

    unsigned char tmpStr[4];

    /* id-MME-UE-S1AP-ID */
    uint16_t protocolIe_Id = id_MME_UE_S1AP_ID;
    copyU16(tmpStr, protocolIe_Id);
    buffer_copy(&g_value_buffer, tmpStr,
                    sizeof(protocolIe_Id));

    uint8_t protocolIe_criticality = CRITICALITY_REJECT;
    buffer_copy(&g_value_buffer, &protocolIe_criticality,
                    sizeof(protocolIe_criticality));

    uint8_t datalen = 2;
    /* TODO needs proper handling*/
    unsigned char mme_ue_id[3];
    datalen = copyU16(mme_ue_id,
            s1apPDU.value.data[0].val.mme_ue_s1ap_id);
    buffer_copy(&g_value_buffer, &datalen, sizeof(datalen));
    buffer_copy(&g_value_buffer, mme_ue_id, datalen);
    /* id-eNB-UE-S1AP-ID */

    protocolIe_Id = id_eNB_UE_S1AP_ID;
    copyU16(tmpStr, protocolIe_Id);
    buffer_copy(&g_value_buffer, tmpStr,
                        sizeof(protocolIe_Id));

    buffer_copy(&g_value_buffer, &protocolIe_criticality,
                    sizeof(protocolIe_criticality));


    /* TODO needs proper handling*/
    unsigned char enb_ue_id[3];
    datalen = copyU16(enb_ue_id,
            s1apPDU.value.data[1].val.enb_ue_s1ap_id);
    buffer_copy(&g_value_buffer, &datalen, sizeof(datalen));
    buffer_copy(&g_value_buffer, enb_ue_id, datalen);

    /* id-NAS-PDU */
    protocolIe_Id = id_NAS_PDU;
    copyU16(tmpStr, protocolIe_Id);
    buffer_copy(&g_value_buffer, tmpStr,
                        sizeof(protocolIe_Id));

    buffer_copy(&g_value_buffer, &protocolIe_criticality,
                    sizeof(protocolIe_criticality));

	log_msg(LOG_INFO, "Received service request reject nas message %d \n",g_mmeS1apInfo->nasMsgSize);
	datalen = g_mmeS1apInfo->nasMsgSize + 1; 

	buffer_copy(&g_buffer, &datalen, sizeof(datalen));

	buffer_copy(&g_buffer, &g_mmeS1apInfo->nasMsgSize, sizeof(uint8_t));

	buffer_copy(&g_buffer, &g_mmeS1apInfo->nasMsgBuf[0], g_mmeS1apInfo->nasMsgSize);

    buffer_copy(&g_buffer, &g_value_buffer.pos,
                        sizeof(g_value_buffer.pos));

    buffer_copy(&g_buffer, &g_value_buffer,
                        g_value_buffer.pos);

    free(s1apPDU.value.data);
    send_sctp_msg(g_mmeS1apInfo->enb_fd, g_buffer.buf, g_buffer.pos,1);

    return SUCCESS;
}

/**
* Get ProtocolIE value for Auth Request sent by mme-app
*/
static int
get_attachReject_protoie_value(struct proto_IE *value, struct commonRej_info *g_attachReqRejInfo)
{
	value->no_of_IEs = AUTH_REQ_NO_OF_IES;

	value->data = (proto_IEs *) malloc(ATTACH_REJECT_NO_OF_IES*
			sizeof(proto_IEs));

	value->data[0].val.mme_ue_s1ap_id = g_attachReqRejInfo->ue_idx;
	value->data[1].val.enb_ue_s1ap_id = g_attachReqRejInfo->s1ap_enb_ue_id;

	log_msg(LOG_INFO, "mme_ue_s1ap_id %d and enb_ue_s1ap_id %d\n",
			g_attachReqRejInfo->ue_idx, g_attachReqRejInfo->s1ap_enb_ue_id);

	return SUCCESS;
}


/**
* Stage specific message processing.
*/
static int
s1ap_attach_reject_processing(struct commonRej_info *g_attachReqRejInfo)
{
	struct Buffer g_buffer = {0};
	struct Buffer g_value_buffer = {0};
	struct s1ap_PDU s1apPDU = {0};

	/* Assigning values to s1apPDU */
	s1apPDU.procedurecode = id_downlinkNASTransport;
	s1apPDU.criticality = CRITICALITY_IGNORE;

	get_attachReject_protoie_value(&s1apPDU.value,g_attachReqRejInfo);

	/* Copy values to buffer from s1apPDU */

	g_buffer.pos = 0;

	uint8_t initiating_message = 0; /* TODO: Add enum */
	buffer_copy(&g_buffer, &initiating_message,
			sizeof(initiating_message));

	buffer_copy(&g_buffer, &s1apPDU.procedurecode,
			sizeof(s1apPDU.procedurecode));

	buffer_copy(&g_buffer, &s1apPDU.criticality,
			sizeof(s1apPDU.criticality));

	/* Copy values in g_value_buffer */
	g_value_buffer.pos = 0;

	/* TODO remove hardcoded values */
	unsigned char chProtoIENo[3] = {0,0,3};

	buffer_copy(&g_value_buffer, chProtoIENo, 3);

	unsigned char tmpStr[4];

	/* id-MME-UE-S1AP-ID */
	uint16_t protocolIe_Id = id_MME_UE_S1AP_ID;
	copyU16(tmpStr, protocolIe_Id);
	buffer_copy(&g_value_buffer, tmpStr,
					sizeof(protocolIe_Id));

	uint8_t protocolIe_criticality = CRITICALITY_REJECT;
	buffer_copy(&g_value_buffer, &protocolIe_criticality,
					sizeof(protocolIe_criticality));

	uint8_t datalen = 2;

	/* TODO needs proper handling*/
	unsigned char mme_ue_id[3];
	datalen = copyU16(mme_ue_id,
			s1apPDU.value.data[0].val.mme_ue_s1ap_id);
	buffer_copy(&g_value_buffer, &datalen, sizeof(datalen));
	buffer_copy(&g_value_buffer, mme_ue_id, datalen);

	/* id-eNB-UE-S1AP-ID */

	protocolIe_Id = id_eNB_UE_S1AP_ID;
	copyU16(tmpStr, protocolIe_Id);
	buffer_copy(&g_value_buffer, tmpStr,
						sizeof(protocolIe_Id));

	buffer_copy(&g_value_buffer, &protocolIe_criticality,
					sizeof(protocolIe_criticality));


	/* TODO needs proper handling*/
	unsigned char enb_ue_id[3];
	datalen = copyU16(enb_ue_id,
			s1apPDU.value.data[1].val.enb_ue_s1ap_id);
	buffer_copy(&g_value_buffer, &datalen, sizeof(datalen));
	buffer_copy(&g_value_buffer, enb_ue_id, datalen);

	/* id-NAS-PDU */
	protocolIe_Id = id_NAS_PDU;
	copyU16(tmpStr, protocolIe_Id);
	buffer_copy(&g_value_buffer, tmpStr,
						sizeof(protocolIe_Id));

	buffer_copy(&g_value_buffer, &protocolIe_criticality,
					sizeof(protocolIe_criticality));

	log_msg(LOG_INFO, "Received attach reject nas message %d \n",g_attachReqRejInfo->nasMsgSize);
	datalen = g_attachReqRejInfo->nasMsgSize + 1; 

	buffer_copy(&g_buffer, &datalen, sizeof(datalen));

	buffer_copy(&g_buffer, &g_attachReqRejInfo->nasMsgSize, sizeof(uint8_t));

	buffer_copy(&g_buffer, &g_attachReqRejInfo->nasMsgBuf[0], g_attachReqRejInfo->nasMsgSize);

	buffer_copy(&g_buffer, &g_value_buffer.pos,
						sizeof(g_value_buffer.pos));

	buffer_copy(&g_buffer, &g_value_buffer,
						g_value_buffer.pos);

	free(s1apPDU.value.data);
	send_sctp_msg(g_attachReqRejInfo->enb_fd, g_buffer.buf, g_buffer.pos,1);
	return SUCCESS;
}


void*
s1ap_reject_handler(void *data)
{
	
	log_msg(LOG_INFO, "NAS Reject Handler.\n");

	struct commonRej_info *nasReject = (struct commonRej_info *)data;

	switch (nasReject->msg_type)
	{
	case attach_reject:
	{
	    log_msg(LOG_INFO, "Send Attach Reject.\n");
	    s1ap_attach_reject_processing(nasReject);
	    break;
	}
	case service_reject:
	{
	    log_msg(LOG_INFO, "Send Service Reject.\n");
	    s1ap_service_reject_processing(nasReject);
	    break;
	}
	default:
	    break;

	}
	return NULL;
}

