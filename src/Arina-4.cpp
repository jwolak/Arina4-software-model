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

#include "Arina-4.h"

#include "spdlog/spdlog.h"

namespace Arina4SoftwareModel::arina4 {

    Arina4::Arina4() : Arina4(std::make_unique<CPU::Cpu>(), std::make_unique<ArithmeticAndLogicalUnit::ArithmeticAndLogicalUnit>()) {}

    Arina4::Arina4(std::unique_ptr<CPU::Cpu> cpu, std::unique_ptr<ArithmeticAndLogicalUnit::ArithmeticAndLogicalUnit> alu)
        : cpu_(std::move(cpu)), alu_(std::move(alu)) {}

    bool Arina4::StartArina4() {
        spdlog::info("[Arina4] Starting Arina4");

        if (cpu_->StartExecution() == false) {
            spdlog::error("[Arina4] Failed to start CPU execution");
            return false;
        }

        if (alu_->Initialize() == false) {
            spdlog::error("[Arina4] Cannot start ALU: not initialized");
            return false;
        }

        if (alu_->StartArithmeticAndLogicalUnit() == false) {
            spdlog::error("[Arina4] Failed to start ALU execution");
            return false;
        }

        spdlog::info("[Arina4] Successfully started Arina4");
        return true;
    }

    void Arina4::StopArina4() {
        cpu_->StopExecution();
        alu_->StopArithmeticAndLogicalUnit();
    }

    Arina4::~Arina4() {}

}  // namespace Arina4SoftwareModel::arina4