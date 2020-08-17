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
#ifndef MEIIE_H_
#define MEIIE_H_

#include "manual/gtpV2Ie.h"



class MeiIe: public GtpV2Ie {
public:
    MeiIe();
    virtual ~MeiIe();

    bool encodeMeiIe(MsgBuffer &buffer,
                 MeiIeData const &data);
    bool decodeMeiIe(MsgBuffer &buffer,
                 MeiIeData &data, Uint16 length);
    void displayMeiIe_v(MeiIeData const &data,
                 Debug &stream);
};

#endif /* MEIIE_H_ */
