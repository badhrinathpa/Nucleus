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
#ifndef PTIIE_H_
#define PTIIE_H_

#include "manual/gtpV2Ie.h"



class PtiIe: public GtpV2Ie {
public:
    PtiIe();
    virtual ~PtiIe();

    bool encodePtiIe(MsgBuffer &buffer,
                 PtiIeData const &data);
    bool decodePtiIe(MsgBuffer &buffer,
                 PtiIeData &data, Uint16 length);
    void displayPtiIe_v(PtiIeData const &data,
                 Debug &stream);
};

#endif /* PTIIE_H_ */
