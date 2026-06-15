/*-
 * BSD 3-Clause License
 *
 * Copyrights 2026, Janusz Wolak
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#pragma once

#include <cstdint>
#include <string>

#include "AluCommonMessagesJsonKeys.h"
#include "nlohmann/json.hpp"

namespace Arina4SoftwareModel::Common::ALU {

    struct AluReplyMessage {
        std::string operation_code;
        uint32_t result;
        uint32_t operation_sequence_number;
        bool carry_flag;
        bool zero_flag;
        std::string status;  // "success" or "error"
    };

    inline void to_json(nlohmann::json& to_json_obj, const AluReplyMessage& alu_reply_message) {
        to_json_obj = nlohmann::json{{kOperationCodeJsonKey, alu_reply_message.operation_code},
                                     {kResultJsonKey, alu_reply_message.result},
                                     {kOperationSequenceNumberJsonKey, alu_reply_message.operation_sequence_number},
                                     {kCarryFlagJsonKey, alu_reply_message.carry_flag},
                                     {kZeroFlagJsonKey, alu_reply_message.zero_flag},
                                     {kStatusJsonKey, alu_reply_message.status}};
    }

    inline void from_json(const nlohmann::json& from_json_obj, AluReplyMessage& alu_reply_message) {
        from_json_obj.at(kOperationCodeJsonKey).get_to(alu_reply_message.operation_code);
        from_json_obj.at(kResultJsonKey).get_to(alu_reply_message.result);
        from_json_obj.at(kOperationSequenceNumberJsonKey).get_to(alu_reply_message.operation_sequence_number);
        from_json_obj.at(kCarryFlagJsonKey).get_to(alu_reply_message.carry_flag);
        from_json_obj.at(kZeroFlagJsonKey).get_to(alu_reply_message.zero_flag);
        from_json_obj.at(kStatusJsonKey).get_to(alu_reply_message.status);
    }
}  // namespace Arina4SoftwareModel::Common::ALU