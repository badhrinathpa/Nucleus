  
/*
 * Copyright 2019-present Infosys Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */
 
/**************************************
 * tauWfAuthRespValidate.cpp
 * This is an auto generated file.
 * Please do not edit this file.
 * All edits to be made through template source file
 * <TOP-DIR/scripts/SMCodeGen/templates/stateMachineTmpls/state.cpp.tt>
 **************************************/

#include "mmeSmDefs.h"
#include "actionTable.h"
#include "actionHandlers/actionHandlers.h"

#include "mmeStates/tauWfAuthRespValidate.h"	
#include "mmeStates/tauWfSecCmp.h"	
#include "mmeStates/tauWfAia.h"

using namespace mme;
using namespace SM;

/******************************************************************************
* Constructor
******************************************************************************/
TauWfAuthRespValidate::TauWfAuthRespValidate():State(tau_wf_auth_resp_validate)
{
}

/******************************************************************************
* Destructor
******************************************************************************/
TauWfAuthRespValidate::~TauWfAuthRespValidate()
{
}

/******************************************************************************
* creates and returns static instance
******************************************************************************/
TauWfAuthRespValidate* TauWfAuthRespValidate::Instance()
{
        static TauWfAuthRespValidate state;
        return &state;
}

/******************************************************************************
* initializes eventToActionsMap
******************************************************************************/
void TauWfAuthRespValidate::initialize()
{
        {
                ActionTable actionTable;
                actionTable.addAction(&ActionHandlers::sec_mode_cmd_to_ue);
                actionTable.setNextState(TauWfSecCmp::Instance());
                eventToActionsMap.insert(pair<uint16_t, ActionTable>(AUTH_RESP_SUCCESS, actionTable));
        }
        {
                ActionTable actionTable;
                actionTable.addAction(&ActionHandlers::send_air_to_hss);
                actionTable.setNextState(TauWfAia::Instance());
                eventToActionsMap.insert(pair<uint16_t, ActionTable>(AUTH_RESP_SYNC_FAILURE, actionTable));
        }
        {
                ActionTable actionTable;
                actionTable.addAction(&ActionHandlers::send_s1_rel_cmd_to_ue);
                actionTable.addAction(&ActionHandlers::send_tau_reject);
                actionTable.addAction(&ActionHandlers::abort_tau_procedure);
                eventToActionsMap.insert(pair<uint16_t, ActionTable>(AUTH_RESP_FAILURE, actionTable));
        }
}