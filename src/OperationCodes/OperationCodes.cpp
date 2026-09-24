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

#include "OperationCodes/OperationCodes.h"
#include "spdlog/spdlog.h"
#include "OperationCodes/OperationCodesType.h"

namespace Arina4SoftwareModel::ArithmeticAndLogicalUnit::OperationCodes {
    OperationCodes::OperationCodes() = default;

    OperationCodesType OperationCodes::getOperationCode(std::string_view operation_code) const {
        spdlog::trace("Getting operation code for: {}", operation_code);

        if(operation_code == "NOP") {
            spdlog::trace("Operation code is {}", operation_code);
            return OperationCodesType::NOP;
        }

        if(operation_code == "LDI") {
            spdlog::trace("Operation code is {}", operation_code);
            return OperationCodesType::LDI;
        }

        if(operation_code == "ADD") {
            spdlog::trace("Operation code is {}", operation_code);
            return OperationCodesType::ADD;
        }

        if(operation_code == "SUB") {
            spdlog::trace("Operation code is {}", operation_code);
            return OperationCodesType::SUB;
        }

        if(operation_code == "MOV_FROM_REG_TO_ACC") {
            spdlog::trace("Operation code is {}", operation_code);
            return OperationCodesType::MOV_FROM_REG_TO_ACC;
        }

        if(operation_code == "MOV_FROM_ACC_TO_REG") {
            spdlog::trace("Operation code is {}", operation_code);
            return OperationCodesType::MOV_FROM_ACC_TO_REG;
        }

        if(operation_code == "JMP") {
            spdlog::trace("Operation code is {}", operation_code);
            return OperationCodesType::JMP;
        }

        if(operation_code == "JZ") {
            spdlog::trace("Operation code is {}", operation_code);
            return OperationCodesType::JZ;
        }

        if(operation_code == "CALL") {
            spdlog::trace("Operation code is {}", operation_code);
            return OperationCodesType::CALL;
        }

        if(operation_code == "RET") {
            spdlog::trace("Operation code is {}", operation_code);
            return OperationCodesType::RET;
        }

        if(operation_code == "AND") {
            spdlog::trace("Operation code is {}", operation_code);
            return OperationCodesType::AND;
        }

        if(operation_code == "OR") {
            spdlog::trace("Operation code is {}", operation_code);
            return OperationCodesType::OR;
        }

        if(operation_code == "XOR") {
            spdlog::trace("Operation code is {}", operation_code);
            return OperationCodesType::XOR;
        }

        if(operation_code == "INC") {
            spdlog::trace("Operation code is {}", operation_code);
            return OperationCodesType::INC;
        }

        if(operation_code == "DEC") {
            spdlog::trace("Operation code is {}", operation_code);
            return OperationCodesType::DEC;
        }

        spdlog::trace("Operation code is unknown, defaulting to NOP");
        return OperationCodesType::NOP;
    }
}