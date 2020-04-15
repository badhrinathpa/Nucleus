/*
 * Copyright 2019-present Open Networking Foundation
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>


#include "err_codes.h"
#include "options.h"
#include "ipc_api.h"
#include "main.h"
#include "s1ap.h"
#include "s1ap_config.h"
#include "sctp_conn.h"
#include "s1ap_structs.h"
#include "s1ap_msg_codes.h"
#include "msgType.h"


extern ipc_handle ipc_S1ap_Hndl;

int
tau_request_handler(struct proto_IE *s1_tau_req_ies, int enb_fd, bool initial_ue_flag)
{
	struct s1_incoming_msg_data_t req= {0};
    int nas_index = 0;

	log_msg(LOG_INFO, "S1ap received tau Request:--\n");

	req.msg_type = tau_request;
    	req.msg_data.tauReq_Q_msg_m.enb_fd = enb_fd;
    req.ue_idx = INVALID_UE_INDEX;

    for(int i = 0; i < s1_tau_req_ies->no_of_IEs; i++)
    {
        switch(s1_tau_req_ies->data[i].IE_type)
        {
		    case S1AP_IE_ENB_UE_ID:
		    {
			    req.s1ap_enb_ue_id = s1_tau_req_ies->data[i].val.enb_ue_s1ap_id;
		    }break;

		    case S1AP_IE_MME_UE_ID:
		    {
			    req.ue_idx = s1_tau_req_ies->data[i].val.mme_ue_s1ap_id;	
		    }break;

            case S1AP_IE_NAS_PDU:
            {
                nas_pdu_header *hdr = &s1_tau_req_ies->data[i].val.nas.header;
                req.msg_data.tauReq_Q_msg_m.seq_num = hdr->seq_no;
                req.msg_data.tauReq_Q_msg_m.active_flag = hdr->active_flag;
                if(initial_ue_flag)
                {
                    while(nas_index < s1_tau_req_ies->data[i].val.nas.elements_len)
                    {
                        log_msg(LOG_INFO, "nasIndex %d, msgType %d\n",
                                nas_index,
                                s1_tau_req_ies->data[i].val.nas.elements[nas_index].msgType);
                        switch(s1_tau_req_ies->data[i].val.nas.elements[nas_index].msgType)
                        {
                            case NAS_IE_TYPE_EPS_MOBILE_ID_IMSI:
                                {
                                    req.msg_data.tauReq_Q_msg_m.flags = s1_tau_req_ies->data[i].val.nas.flags;
                                    if(UE_ID_IMSI(s1_tau_req_ies->data[i].val.nas.flags))
                                    { 
                                        log_msg(LOG_INFO, "Mobile Id IMSI\n");
                                        memcpy(&(req.msg_data.tauReq_Q_msg_m.IMSI), 
                                               &(s1_tau_req_ies->data[i].val.nas.elements[nas_index].pduElement.IMSI),
                                               BINARY_IMSI_LEN);
                                    }
                                    else if(UE_ID_GUTI(s1_tau_req_ies->data[i].val.nas.flags))
                                    {
                                        log_msg(LOG_INFO, "Mobile Id GUTI\n");
                                        memcpy(&(req.msg_data.tauReq_Q_msg_m.mi_guti), 
                                            &(s1_tau_req_ies->data[i].val.nas.elements[nas_index].pduElement.mi_guti),
                                               sizeof(struct guti));
                                    }
                                    break;
                                }
                            case NAS_IE_TYPE_EPS_BEARER_STATUS:
                                {
                                    memcpy(req.msg_data.tauReq_Q_msg_m.eps_bearer_status,
                                        s1_tau_req_ies->data[i].val.nas.elements[nas_index].pduElement.eps_bearer_status, BEARER_STATUS_LEN);

                                }break;
                            default:
                                {
                                    log_msg(LOG_INFO, "nas element not handled\n");
                                }
                        }

                        nas_index++;
                    }
                }
            }break;
            default:
			    // Once MME starts handlign this request we can parse and send the content 
			    log_msg(LOG_WARNING,"Unhandled IE In tau request %d",s1_tau_req_ies->data[i].IE_type);
		}
	}
	
	req.destInstAddr = htonl(mmeAppInstanceNum_c);
    	req.srcInstAddr = htonl(s1apAppInstanceNum_c);
    	send_tipc_message(ipc_S1ap_Hndl, mmeAppInstanceNum_c, (char *)&req, S1_READ_MSG_BUF_SIZE);


	log_msg(LOG_INFO, "Sent TAU request to mme-app\n");
	return SUCCESS;
}

