 /*
 * Copyright 2020-present Infosys Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */ 
/******************************************************************************
 * smDedActProcCtxtManager.cpp
 * This is an auto generated file.
 * Please do not edit this file.
 * All edits to be made through template source file
 * <TOP-DIR/scripts/SMCodeGen/templates/ctxtManagerTmpls/blockPoolManager.cpp.tt>
 ******************************************************************************/

#include "memPoolManager.h"
#include "contextManager/dataBlocks.h"
#include "contextManager/smDedActProcCtxtManager.h"

using namespace cmn::memPool;

namespace mme
{
	/******************************************************************************
	* Constructor
	******************************************************************************/
	SmDedActProcCtxtManager::SmDedActProcCtxtManager(int numOfBlocks):poolManager_m(numOfBlocks)
	{
	}
	
	/******************************************************************************
	* Destructor
	******************************************************************************/
	SmDedActProcCtxtManager::~SmDedActProcCtxtManager()
	{
	}
	
	/******************************************************************************
	* Allocate SmDedActProcCtxt data block
	******************************************************************************/
	SmDedActProcCtxt* SmDedActProcCtxtManager::allocateSmDedActProcCtxt()
	{
		SmDedActProcCtxt* SmDedActProcCtxt_p = poolManager_m.allocate();
		return SmDedActProcCtxt_p;
	}
	
	/******************************************************************************
	* Deallocate a SmDedActProcCtxt data block
	******************************************************************************/
	void SmDedActProcCtxtManager::deallocateSmDedActProcCtxt(SmDedActProcCtxt* SmDedActProcCtxtp )
	{
		poolManager_m.free( SmDedActProcCtxtp );
	}
}