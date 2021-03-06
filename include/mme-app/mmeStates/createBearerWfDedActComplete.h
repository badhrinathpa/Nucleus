/*
 * Copyright 2020-present Infosys Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */
 
 /******************************************************
 * createBearerWfDedActComplete.h
 * This is an auto generated file.
 * Please do not edit this file.
 * All edits to be made through template source file
 * <TOP-DIR/scripts/SMCodeGen/templates/stateMachineTmpls/state.h.tt>
 ******************************************************/
 
#ifndef __CreateBearerWfDedActComplete__
#define __CreateBearerWfDedActComplete__

#include "state.h"

namespace mme {

	class CreateBearerWfDedActComplete : public SM::State
	{
		public:
			/******************************************
			* Instance 
			*    Creates static instance for the state
			*******************************************/
			static CreateBearerWfDedActComplete* Instance();

			/****************************************
			* CreateBearerWfDedActComplete
			*    Destructor
			****************************************/
			~CreateBearerWfDedActComplete();			
			
			/******************************************
			* initialize
			*  Initializes action handlers for the state
			* and next state
			******************************************/
			void initialize();
	
		private:
			/****************************************
			* CreateBearerWfDedActComplete
			*    Private constructor
			****************************************/
			CreateBearerWfDedActComplete();  
	};
};
#endif // __CreateBearerWfDedActComplete__
