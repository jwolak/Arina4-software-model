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

#include "ArithmeticAndLogicalUnit/ArithmeticAndLogicalUnit.h"

#include "ArithmeticAndLogicalUnit/ALUExecutor/ALUExecutor.h"
#include "Common/ALU/AluReplyMessage.h"
#include "Common/ALU/AluRequestMessage.h"
#include "Common/HerkusBusTopics.h"
#include "spdlog/spdlog.h"

namespace Arina4SoftwareModel::ArithmeticAndLogicalUnit {
    ArithmeticAndLogicalUnit::ArithmeticAndLogicalUnit() : ArithmeticAndLogicalUnit(std::make_unique<ALUExecutor::ALUExecutor>()) {}

    ArithmeticAndLogicalUnit::ArithmeticAndLogicalUnit(std::unique_ptr<ALUExecutor::IALUExecutor> alu_executor)
        : is_initialized_(false), herkus_bus_(Herkus::HerkusBus::getInstance()), alu_executor_(std::move(alu_executor)) {}

    bool ArithmeticAndLogicalUnit::Initialize() {
        spdlog::info("ArithmeticAndLogicalUnit Initialize called");

        spdlog::info("Subscribe to the ALU topic on the HerkusBus");
        herkus_bus_.Subscribe(Common::HerkusBusTopics::kAluTopic, [this](const std::string& topic, const nlohmann::json& message_payload) {
            spdlog::info("Received message on topic {}: {}", topic, message_payload.dump());
            try {
                Common::ALU::AluRequestMessage alu_request_message;
                try {
                    spdlog::debug("Attempting to parse ALU request message from JSON");
                    alu_request_message = message_payload.get<Common::ALU::AluRequestMessage>();
                } catch (const nlohmann::json::exception& ex) {
                    spdlog::error("ALU JSON parse error: {}", ex.what());
                    return;
                }

                spdlog::debug("Parsed ALU request message: operation_code={}, acc={}, operand_b={}, operation_sequence_number={}",
                              alu_request_message.operation_code, alu_request_message.acc, alu_request_message.operand_b,
                              alu_request_message.operation_sequence_number);
                Common::ALU::AluReplyMessage resp =
                    alu_executor_->Execute(alu_request_message.operation_code, alu_request_message.acc, alu_request_message.operand_b);

                spdlog::debug("ALU execution completed. Publishing response to topic {}: {}", Common::HerkusBusTopics::kAluTopic, nlohmann::json(resp).dump());
                herkus_bus_.Publish(Common::HerkusBusTopics::kAluTopic, nlohmann::json(resp));
            } catch (const std::exception& ex) {
                spdlog::error("Error processing ALU request: {}", ex.what());
            }
        });

        is_initialized_ = true;
        spdlog::debug("is_initialized_ set to true");

        spdlog::info("ArithmeticAndLogicalUnit initialized successfully");
        return true;
    }

}  // namespace Arina4SoftwareModel::ArithmeticAndLogicalUnit