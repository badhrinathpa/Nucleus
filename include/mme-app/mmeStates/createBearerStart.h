/*
 * Copyright 2020-present Infosys Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */
 
 /******************************************************
 * createBearerStart.h
 * This is an auto generated file.
 * Please do not edit this file.
 * All edits to be made through template source file
 * <TOP-DIR/scripts/SMCodeGen/templates/stateMachineTmpls/state.h.tt>
 ******************************************************/
 
#ifndef __CreateBearerStart__
#define __CreateBearerStart__

#include "state.h"

namespace mme {

	class CreateBearerStart : public SM::State
	{
		public:
			/******************************************
			* Instance 
			*    Creates static instance for the state
			*******************************************/
			static CreateBearerStart* Instance();

			/****************************************
			* CreateBearerStart
			*    Destructor
			****************************************/
			~CreateBearerStart();			
			
			/******************************************
			* initialize
			*  Initializes action handlers for the state
			* and next state
			******************************************/
			void initialize();
	
		private:
			/****************************************
			* CreateBearerStart
			*    Private constructor
			****************************************/
			CreateBearerStart();  
	};
};
#endif // __CreateBearerStart__
