
/*
 * Copyright 2020-present Infosys Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */
 
/**************************************
 * s1ReleaseWfUeCtxtReleaseComp.cpp
 * This is an auto generated file.
 * Please do not edit this file.
 * All edits to be made through template source file
 * <TOP-DIR/scripts/SMCodeGen/templates/stateMachineTmpls/state.cpp.tt>
 **************************************/


#include "actionTable.h"
#include "actionHandlers/actionHandlers.h"
#include "mmeSmDefs.h"
#include "utils/mmeStatesUtils.h"
#include "utils/mmeTimerTypes.h"

#include "mmeStates/s1ReleaseWfUeCtxtReleaseComp.h"

using namespace mme;
using namespace SM;

/******************************************************************************
* Constructor
******************************************************************************/
S1ReleaseWfUeCtxtReleaseComp::S1ReleaseWfUeCtxtReleaseComp():State(s1_release_wf_ue_ctxt_release_comp)
{
        stateEntryAction = &MmeStatesUtils::on_state_entry;
        stateExitAction = &MmeStatesUtils::on_state_exit;
        eventValidator = &MmeStatesUtils::validate_event;
}

/******************************************************************************
* Destructor
******************************************************************************/
S1ReleaseWfUeCtxtReleaseComp::~S1ReleaseWfUeCtxtReleaseComp()
{
}

/******************************************************************************
* creates and returns static instance
******************************************************************************/
S1ReleaseWfUeCtxtReleaseComp* S1ReleaseWfUeCtxtReleaseComp::Instance()
{
        static S1ReleaseWfUeCtxtReleaseComp state;
        return &state;
}

/******************************************************************************
* initializes eventToActionsMap
******************************************************************************/
void S1ReleaseWfUeCtxtReleaseComp::initialize()
{
        {
                ActionTable actionTable;
                actionTable.addAction(&ActionHandlers::process_ue_ctxt_rel_comp);
                eventToActionsMap.insert(pair<uint16_t, ActionTable>(UE_CTXT_REL_COMP_FROM_ENB, actionTable));
        }
}
