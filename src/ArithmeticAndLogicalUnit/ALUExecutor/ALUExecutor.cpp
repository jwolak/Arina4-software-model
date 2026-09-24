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

#include "ArithmeticAndLogicalUnit/ALUExecutor/ALUExecutor.h"

#include "Common/ALU/AluReplyMessage.h"
#include "OperationCodes/OperationCodes.h"
#include "spdlog/spdlog.h"

namespace Arina4SoftwareModel::ArithmeticAndLogicalUnit::ALUExecutor {

    ALUExecutor::ALUExecutor() : ALUExecutor(std::make_unique<OperationCodes::OperationCodes>()) {}

    /* Tests purpose constructor */
    ALUExecutor::ALUExecutor(std::unique_ptr<OperationCodes::OperationCodes> operation_codes) : operation_codes_(std::move(operation_codes)) {}

    Common::ALU::AluReplyMessage ALUExecutor::Execute(const std::string& operation_code, uint32_t acc, uint32_t operand_b) {
        spdlog::info("Executing ALU operation: {} with acc={} and operand_b={}", operation_code, acc, operand_b);

        OperationCodes::OperationCodesType operation_code_type = operation_codes_->getOperationCode(operation_code);

        Common::ALU::AluReplyMessage alu_reply_message {};

        switch (operation_code_type) {
            case OperationCodes::OperationCodesType::NOP:
                spdlog::debug("NOP operation executed");
                alu_reply_message.carry_flag = false;
                alu_reply_message.program_counter_changed = false;
                alu_reply_message.operation_code = operation_code;
                alu_reply_message.result = acc;
                alu_reply_message.status = "OK";
                alu_reply_message.zero_flag = (acc == 0);
                break;

            case OperationCodes::OperationCodesType::LDI:
                spdlog::debug("LDI operation executed");
                acc = operand_b;
                break;

            case OperationCodes::OperationCodesType::ADD:
                spdlog::debug("ADD operation executed");
                acc = acc + operand_b;
                break;

            case OperationCodes::OperationCodesType::SUB:
                spdlog::debug("SUB operation executed");
                acc = acc - operand_b;
                break;

            case OperationCodes::OperationCodesType::MOV_FROM_REG_TO_ACC:
                spdlog::debug("MOV_FROM_REG_TO_ACC operation executed");
                acc = operand_b;
                break;

            case OperationCodes::OperationCodesType::MOV_FROM_ACC_TO_REG:
                spdlog::debug("MOV_FROM_ACC_TO_REG operation executed");
                operand_b = acc;
                break;

            case OperationCodes::OperationCodesType::JMP:
                spdlog::debug("JMP operation executed");
                alu_reply_message.carry_flag = false;
                alu_reply_message.program_counter_changed = true;
                alu_reply_message.program_counter = operand_b;
                alu_reply_message.operation_code = operation_code;
                alu_reply_message.result = acc;
                alu_reply_message.status = "OK";
                alu_reply_message.zero_flag = (acc == 0);
                break;

            case OperationCodes::OperationCodesType::JZ:
                spdlog::debug("JZ operation executed");
                break;

            case OperationCodes::OperationCodesType::JC:
                spdlog::debug("JC operation executed");
                break;

            case OperationCodes::OperationCodesType::CALL:
                spdlog::debug("CALL operation executed");
                break;

            case OperationCodes::OperationCodesType::RET:
                spdlog::debug("RET operation executed");
                break;

            case OperationCodes::OperationCodesType::AND:
                spdlog::debug("AND operation executed");
                acc = acc & operand_b;
                break;

            case OperationCodes::OperationCodesType::OR:
                spdlog::debug("OR operation executed");
                acc = acc | operand_b;
                break;

            case OperationCodes::OperationCodesType::XOR:
                spdlog::debug("XOR operation executed");
                acc = acc ^ operand_b;
                break;

            case OperationCodes::OperationCodesType::INC:
                spdlog::debug("INC operation executed");
                acc = acc + 1;
                break;

            case OperationCodes::OperationCodesType::DEC:
                spdlog::debug("DEC operation executed");
                acc = acc - 1;
                break;

            default:
                spdlog::error("Unknown operation code type");
                break;
        }

        return alu_reply_message;
    }
}  // namespace Arina4SoftwareModel::ArithmeticAndLogicalUnit::ALUExecutor