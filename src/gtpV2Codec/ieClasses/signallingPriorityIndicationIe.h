/*
 * Copyright (c) 2020, Infosys Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
 /******************************************************************************
 *
 * This is an auto generated file.
 * Please do not edit this file.
 * All edits to be made through template source file
 * <TOP-DIR/scripts/GtpV2StackCodeGen/tts/ietemplate.h.tt>
 ******************************************************************************/
#ifndef SIGNALLINGPRIORITYINDICATIONIE_H_
#define SIGNALLINGPRIORITYINDICATIONIE_H_

#include "manual/gtpV2Ie.h"



class SignallingPriorityIndicationIe: public GtpV2Ie {
public:
    SignallingPriorityIndicationIe();
    virtual ~SignallingPriorityIndicationIe();

    bool encodeSignallingPriorityIndicationIe(MsgBuffer &buffer,
                 SignallingPriorityIndicationIeData const &data);
    bool decodeSignallingPriorityIndicationIe(MsgBuffer &buffer,
                 SignallingPriorityIndicationIeData &data, Uint16 length);
    void displaySignallingPriorityIndicationIe_v(SignallingPriorityIndicationIeData const &data,
                 Debug &stream);
};

#endif /* SIGNALLINGPRIORITYINDICATIONIE_H_ */
