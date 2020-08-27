/*
 * Copyright (c) 2019, Infosys Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cmath>
#include <controlBlock.h>
#include <contextManager/subsDataGroupManager.h>
#include <log.h>
#include <mmeStates/detachStart.h>
#include <mmeStates/niDetachStart.h>
#include <mmeStates/pagingStart.h>
#include <mmeStates/serviceRequestStart.h>
#include <mmeStates/tauStart.h>
#include <mmeStates/erabModIndStart.h>
#include <utils/mmeContextManagerUtils.h>
#include <mmeStates/intraS1HoStart.h>
#include <utils/mmeCommonUtils.h>
#include <utils/mmeTimerUtils.h>
#include "mmeStatsPromClient.h"

#define BEARER_ID_OFFSET    4
#define FIRST_SET_BIT(bits)    log2(bits & -bits) + 1
#define CLEAR_BIT(bits,pos)      bits &= ~(1 << pos)
#define SET_BIT(bits,pos)        bits |= (1 << pos)

using namespace mme;

MmeDetachProcedureCtxt* MmeContextManagerUtils::allocateDetachProcedureCtxt(SM::ControlBlock& cb_r, DetachType detachType)
{
    log_msg(LOG_DEBUG, "allocateDetachProcedureCtxt: Entry");

    MmeDetachProcedureCtxt *prcdCtxt_p =
            SubsDataGroupManager::Instance()->getMmeDetachProcedureCtxt();

    if (prcdCtxt_p != NULL)
    {
        prcdCtxt_p->setCtxtType(ProcedureType::detach_c);
        prcdCtxt_p->setDetachType(detachType);

        if (detachType == ueInitDetach_c)
        {
            mmeStats::Instance()->increment(mmeStatsCounter::MME_PROCEDURES_DETACH_PROC_UE_INIT);
            prcdCtxt_p->setNextState(DetachStart::Instance());
        }
        else
        {
            mmeStats::Instance()->increment(mmeStatsCounter::MME_PROCEDURES_DETACH_PROC_NETWORK_INIT);
            prcdCtxt_p->setNextState(NiDetachStart::Instance());
        }

        cb_r.setCurrentTempDataBlock(prcdCtxt_p);
    }

    return prcdCtxt_p;
}

MmeSvcReqProcedureCtxt*
MmeContextManagerUtils::allocateServiceRequestProcedureCtxt(SM::ControlBlock& cb_r, PagingTrigger pagingTrigger)
{
    log_msg(LOG_DEBUG, "allocateServiceRequestProcedureCtxt: Entry");

    MmeSvcReqProcedureCtxt *prcdCtxt_p =
            SubsDataGroupManager::Instance()->getMmeSvcReqProcedureCtxt();
    if (prcdCtxt_p != NULL)
    {
        prcdCtxt_p->setCtxtType(ProcedureType::serviceRequest_c);
        prcdCtxt_p->setPagingTrigger(pagingTrigger);

        if (pagingTrigger == ddnInit_c)
        {
            mmeStats::Instance()->increment(mmeStatsCounter::MME_PROCEDURES_SERVICE_REQUEST_PROC_DDN_INIT);
            prcdCtxt_p->setNextState(PagingStart::Instance());
        }
        else
        {
            mmeStats::Instance()->increment(mmeStatsCounter::MME_PROCEDURES_SERVICE_REQUEST_PROC_UE_INIT);
            prcdCtxt_p->setNextState(ServiceRequestStart::Instance());
        }

        cb_r.setCurrentTempDataBlock(prcdCtxt_p);

    }
    return prcdCtxt_p;
}

MmeTauProcedureCtxt*
MmeContextManagerUtils::allocateTauProcedureCtxt(SM::ControlBlock& cb_r)
{
    log_msg(LOG_DEBUG, "allocateTauRequestProcedureCtxt: Entry");

    MmeTauProcedureCtxt *prcdCtxt_p =
            SubsDataGroupManager::Instance()->getMmeTauProcedureCtxt();
    if (prcdCtxt_p != NULL)
    {
        mmeStats::Instance()->increment(mmeStatsCounter::MME_PROCEDURES_TAU_PROC);
        prcdCtxt_p->setCtxtType(ProcedureType::tau_c);
        prcdCtxt_p->setNextState(TauStart::Instance());

        cb_r.setCurrentTempDataBlock(prcdCtxt_p);
    }

    return prcdCtxt_p;
}

MmeErabModIndProcedureCtxt*
MmeContextManagerUtils::allocateErabModIndProcedureCtxt(SM::ControlBlock& cb_r)
{
    log_msg(LOG_DEBUG, "allocateErabModIndRequestProcedureCtxt: Entry");

    MmeErabModIndProcedureCtxt *prcdCtxt_p =
            SubsDataGroupManager::Instance()->getMmeErabModIndProcedureCtxt();
    if (prcdCtxt_p != NULL)
    {
        mmeStats::Instance()->increment(mmeStatsCounter::MME_PROCEDURES_ERAB_MOD_IND_PROC);
        prcdCtxt_p->setCtxtType(ProcedureType::erabModInd_c);
        prcdCtxt_p->setNextState(ErabModIndStart::Instance());

        cb_r.setCurrentTempDataBlock(prcdCtxt_p);
    }

    return prcdCtxt_p;
}

S1HandoverProcedureContext* MmeContextManagerUtils::allocateHoContext(SM::ControlBlock& cb_r)
{
    log_msg(LOG_DEBUG, "allocateHoProcedureCtxt: Entry");

    S1HandoverProcedureContext *prcdCtxt_p =
            SubsDataGroupManager::Instance()->getS1HandoverProcedureContext();
    if (prcdCtxt_p != NULL)
    {
        mmeStats::Instance()->increment(mmeStatsCounter::MME_PROCEDURES_S1_ENB_HANDOVER_PROC);
        prcdCtxt_p->setCtxtType(ProcedureType::s1Handover_c);
        prcdCtxt_p->setNextState(IntraS1HoStart::Instance());
        prcdCtxt_p->setHoType(intraMmeS1Ho_c);
        cb_r.setCurrentTempDataBlock(prcdCtxt_p);
    }

    return prcdCtxt_p;
}

bool MmeContextManagerUtils::deleteProcedureCtxt(MmeProcedureCtxt* procedure_p)
{
	log_msg(LOG_DEBUG, "deleteProcedureCtxt: Entry");

	if (procedure_p == NULL)
	{
		log_msg(LOG_INFO, "Procedure Context is NULL");

		return false;
	}

	SubsDataGroupManager* subsDgMgr_p = SubsDataGroupManager::Instance();

	log_msg(LOG_INFO, "Procedure Type is %d", procedure_p->getCtxtType());

	bool rc = true;
	switch (procedure_p->getCtxtType())
	{
		case attach_c:
                {
                        MmeAttachProcedureCtxt* atchProc_p =
                                static_cast<MmeAttachProcedureCtxt *>(procedure_p);

                        subsDgMgr_p->deleteMmeAttachProcedureCtxt(atchProc_p);
                        break;
                }
                case s1Release_c:
                {
                        MmeS1RelProcedureCtxt* s1RelProc_p =
                                static_cast<MmeS1RelProcedureCtxt *>(procedure_p);

                        subsDgMgr_p->deleteMmeS1RelProcedureCtxt(s1RelProc_p);
                        break;
                }	
		case detach_c:
		{
			MmeDetachProcedureCtxt* detachProc_p = 
				static_cast<MmeDetachProcedureCtxt *>(procedure_p);

			subsDgMgr_p->deleteMmeDetachProcedureCtxt(detachProc_p);

			break;
		}
		case serviceRequest_c:
		{
			MmeSvcReqProcedureCtxt* svcReqProc_p =
					static_cast<MmeSvcReqProcedureCtxt*>(procedure_p);

			subsDgMgr_p->deleteMmeSvcReqProcedureCtxt(svcReqProc_p);

			break;
		}
		case tau_c:
		{
			MmeTauProcedureCtxt* tauProc_p =
					static_cast<MmeTauProcedureCtxt*>(procedure_p);

			subsDgMgr_p->deleteMmeTauProcedureCtxt(tauProc_p);

			break;
		}
		case s1Handover_c:
		{
			S1HandoverProcedureContext* s1HoProc_p =
					static_cast<S1HandoverProcedureContext*>(procedure_p);

			subsDgMgr_p->deleteS1HandoverProcedureContext(s1HoProc_p);

			break;
		}
		case erabModInd_c:
		{
			MmeErabModIndProcedureCtxt* erabModIndProc_p =
                    			static_cast<MmeErabModIndProcedureCtxt*>(procedure_p);
			
			subsDgMgr_p->deleteMmeErabModIndProcedureCtxt(erabModIndProc_p);

			break;
		}
		default:
		{
			log_msg(LOG_INFO, "Unsupported procedure type %d\n", procedure_p->getCtxtType());
			rc = false;
		}
	}
	return rc;
}

bool MmeContextManagerUtils::deallocateProcedureCtxt(SM::ControlBlock& cb_r, ProcedureType procType)
{
    bool rc = false;
    
    MmeProcedureCtxt* procedure_p =
			static_cast<MmeProcedureCtxt*>(cb_r.getTempDataBlock());

    MmeProcedureCtxt* prevProcedure_p = NULL;
    MmeProcedureCtxt* nextProcedure_p = NULL;

    while (procedure_p != NULL)
    {
        nextProcedure_p =
            static_cast<MmeProcedureCtxt*>(procedure_p->getNextTempDataBlock());
        
        ProcedureType procedureType = procedure_p->getCtxtType();
        if (procType == procedureType)
        {
            log_msg(LOG_INFO, "Procedure type %d\n", procedureType);

	    // Stop procedure specific timers 
	    // Stop state guard timer if its running
    	    MmeTimerUtils::stopTimer(procedure_p->getStateGuardTimerCtxt());
            
	    rc = deleteProcedureCtxt(procedure_p);
            
            if (rc == true)
            {
                if (prevProcedure_p != NULL)
                {
                    if (nextProcedure_p != NULL)
                    {
                        prevProcedure_p->setNextTempDataBlock(nextProcedure_p);
                    }
                }
                else
                {
                    cb_r.setTempDataBlock(nextProcedure_p);
                }
            }
            // break out of while loop
            break;
        }
        prevProcedure_p = procedure_p;
        procedure_p = nextProcedure_p;		
    }

    return rc;
}

bool MmeContextManagerUtils::deallocateAllProcedureCtxts(SM::ControlBlock& cb_r)
{
    bool rc = false;

    MmeProcedureCtxt* procedure_p =
    		static_cast<MmeProcedureCtxt*>(cb_r.getTempDataBlock());
    
    MmeProcedureCtxt* nextProcedure_p = NULL;
    
    while (procedure_p != NULL)
    {
        nextProcedure_p =
    		static_cast<MmeProcedureCtxt*>(procedure_p->getNextTempDataBlock());
    
        if (procedure_p->getCtxtType() != defaultMmeProcedure_c)
        {
            rc = deleteProcedureCtxt(procedure_p);
        }

        procedure_p = nextProcedure_p;
    }
    return rc;
}

MmeProcedureCtxt* MmeContextManagerUtils::findProcedureCtxt(SM::ControlBlock& cb_r, ProcedureType procType)
{
    MmeProcedureCtxt* mmeProcCtxt_p = NULL;

    MmeProcedureCtxt* currentProcedure_p =
                static_cast<MmeProcedureCtxt*>(cb_r.getTempDataBlock());

    MmeProcedureCtxt* nextProcedure_p = NULL;

    while (currentProcedure_p != NULL)
    {
        nextProcedure_p = static_cast<MmeProcedureCtxt*>(
                currentProcedure_p->getNextTempDataBlock());

        if (currentProcedure_p->getCtxtType() == procType)
        {
            mmeProcCtxt_p = currentProcedure_p;
            break;
        }
        currentProcedure_p = nextProcedure_p;
    }

    return mmeProcCtxt_p;
}

void MmeContextManagerUtils::deleteAllSessionContext(SM::ControlBlock& cb_r)
{
    UEContext *ueCtxt_p = static_cast<UEContext*>(cb_r.getPermDataBlock());
    if (ueCtxt_p == NULL)
    {
        log_msg(LOG_DEBUG, "Failed to retrieve UEContext from control block %u",
                cb_r.getCBIndex());
        return;
    }

    auto &sessionCtxtContainer = ueCtxt_p->getSessionContextContainer();
    if (sessionCtxtContainer.size() < 1)
    {
        log_msg(LOG_ERROR, "Session context list is empty for UE IDX %d\n",
                cb_r.getCBIndex());
        return;
    }

    auto it = sessionCtxtContainer.begin();
    SessionContext *session_p = NULL;
    while (it != sessionCtxtContainer.end())
    {
        session_p = *it;
        it++;
        deallocateSessionContext(cb_r, session_p, ueCtxt_p);
    }
}

void MmeContextManagerUtils::deleteUEContext(uint32_t cbIndex, bool deleteControlBlockFlag)
{
    SM::ControlBlock* cb_p = SubsDataGroupManager::Instance()->findControlBlock(cbIndex);
    if (cb_p == NULL)
    {
        log_msg(LOG_DEBUG, "Failed to find control block for index %u", cbIndex);
        return;
    }

    deallocateAllProcedureCtxts(*cb_p);

    deleteAllSessionContext(*cb_p);

    UEContext* ueCtxt_p = static_cast<UEContext *>(cb_p->getPermDataBlock());
    if (ueCtxt_p == NULL)
    {
        log_msg(LOG_DEBUG, "Failed to retrieve UEContext from control block %u", cbIndex);
    }
    else
    {
        MmContext* mmContext_p = ueCtxt_p->getMmContext();
        if (mmContext_p != NULL)
        {
            SubsDataGroupManager::Instance()->deleteMmContext(mmContext_p);
            ueCtxt_p->setMmContext(NULL);
        }

        // Remove IMSI -> CBIndex key mapping
        const DigitRegister15& ue_imsi = ueCtxt_p->getImsi();
        SubsDataGroupManager::Instance()->deleteimsikey(ue_imsi);

        // Remove mTMSI -> CBIndex mapping
        SubsDataGroupManager::Instance()->deletemTmsikey(ueCtxt_p->getMTmsi());

        SubsDataGroupManager::Instance()->deleteUEContext(ueCtxt_p);
        cb_p->setPermDataBlock(NULL);
    }

    if (deleteControlBlockFlag)
        SubsDataGroupManager::Instance()->deAllocateCB(cb_p->getCBIndex());
}

SessionContext*
MmeContextManagerUtils::allocateSessionContext(SM::ControlBlock &cb_r,
        UEContext &ueCtxt)
{
    SessionContext *sessionCtxt_p =
            SubsDataGroupManager::Instance()->getSessionContext();
    if (sessionCtxt_p != NULL)
    {
        BearerContext *bearerCtxt_p =
                MmeContextManagerUtils::allocateBearerContext(cb_r, ueCtxt,
                        *sessionCtxt_p);
        if (bearerCtxt_p == NULL)
        {
            log_msg(LOG_DEBUG, "Failed to allocate bearer context");

            SubsDataGroupManager::Instance()->deleteSessionContext(
                    sessionCtxt_p);

            return NULL;
        }
        sessionCtxt_p->setLinkedBearerId(bearerCtxt_p->getBearerId());
        ueCtxt.addSessionContext(sessionCtxt_p);
    }
    return sessionCtxt_p;
}

BearerContext*
MmeContextManagerUtils::allocateBearerContext(SM::ControlBlock &cb_r,
        UEContext &uectxt, SessionContext &sessionCtxt)
{
    BearerContext *bearerCtxt_p = NULL;
    uint16_t bitmap = uectxt.getBearerIdBitMap();

    // 0x7FF : All bearers ids are allocated.
    uint8_t id = (bitmap == 0x7FF) ? 0 : (FIRST_SET_BIT(~bitmap));
    if (id > 0 && id <= 11)
    {
        bearerCtxt_p = SubsDataGroupManager::Instance()->getBearerContext();
        if (bearerCtxt_p != NULL)
        {
            bearerCtxt_p->setBearerId(id + BEARER_ID_OFFSET); // Bearer id start 5
            id--;
            SET_BIT(bitmap, id);
            uectxt.setBearerIdBitMap(bitmap);
            sessionCtxt.addBearerContext(bearerCtxt_p);
        }
    }

    return bearerCtxt_p;
}

void MmeContextManagerUtils::deallocateSessionContext(SM::ControlBlock &cb_r,
        SessionContext *sessionCtxt_p, UEContext* ueContext_p)
{
    if (sessionCtxt_p != NULL)
    {
        if (ueContext_p != NULL)
            ueContext_p ->removeSessionContext(sessionCtxt_p);

        auto &bearerCtxtContainer = sessionCtxt_p->getBearerContextContainer();
        if (bearerCtxtContainer.size() < 1)
        {
            log_msg(LOG_ERROR, "Bearer context list is empty for UE IDX %d\n",
                    cb_r.getCBIndex());
            return;
        }
        auto it = bearerCtxtContainer.begin();
        BearerContext *bearer_p = NULL;
        while (it != bearerCtxtContainer.end())
        {
            bearer_p = *it;
            it++;
            deallocateBearerContext(cb_r, bearer_p, sessionCtxt_p, ueContext_p);
        }

        SubsDataGroupManager::Instance()->deleteSessionContext(sessionCtxt_p);
    }
}

void MmeContextManagerUtils::deallocateBearerContext(SM::ControlBlock &cb_r,
        BearerContext *bearerCtxt_p, SessionContext *sessionCtxt_p, UEContext *ueCtxt_p)
{
    if (bearerCtxt_p != NULL)
    {
        // Remove from bearer context container
        if (sessionCtxt_p != NULL)
            sessionCtxt_p->removeBearerContext(bearerCtxt_p);

        // clear the id in the bitmap
        if (ueCtxt_p != NULL)
        {
            uint16_t bitmap = ueCtxt_p->getBearerIdBitMap();
            uint8_t bearerId = bearerCtxt_p->getBearerId();
            if (bearerId >= 5 && bearerId <= 15)
            {
                uint8_t id = bearerId - BEARER_ID_OFFSET - 1;
                CLEAR_BIT(bitmap, id);
                ueCtxt_p->setBearerIdBitMap(bitmap);
            }
        }
        SubsDataGroupManager::Instance()->deleteBearerContext(bearerCtxt_p);
    }
}

