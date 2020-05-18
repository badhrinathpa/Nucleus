/*
 * Copyright 2020-present Infosys Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */
 
 /******************************************************
 * niDetachWfS1RelComp.h
 * This is an auto generated file.
 * Please do not edit this file.
 * All edits to be made through template source file
 * <TOP-DIR/scripts/SMCodeGen/templates/stateMachineTmpls/state.h.tt>
 ******************************************************/
 
#ifndef __NiDetachWfS1RelComp__
#define __NiDetachWfS1RelComp__

#include "state.h"

namespace mme {

	class NiDetachWfS1RelComp : public SM::State
	{
		public:
			/******************************************
			* Instance 
			*    Creates static instance for the state
			*******************************************/
			static NiDetachWfS1RelComp* Instance();

			/****************************************
			* NiDetachWfS1RelComp
			*    Destructor
			****************************************/
			~NiDetachWfS1RelComp();			
			
			/******************************************
			* initialize
			*  Initializes action handlers for the state
			* and next state
			******************************************/
			void initialize();
	
		private:
			/****************************************
			* NiDetachWfS1RelComp
			*    Private constructor
			****************************************/
			NiDetachWfS1RelComp();  
	};
};
#endif // __NiDetachWfS1RelComp__
