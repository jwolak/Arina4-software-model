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

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include "Common/Registers/RegistersMessage.h"
#include "HerkusBus.hpp"
#include "Registers.h"

namespace Arina4SoftwareModel::Regs {
    class RegistersMux {
      public:
        RegistersMux();
        bool Initialize();
        bool StartRegistersMux();
        void StopRegistersMux();

      protected:
        explicit RegistersMux(Herkus::IHerkusBus& herkus_bus, std::unique_ptr<Regs::IRegisters> registers);
        void RegistersMuxLoop();

      private:
        bool is_initialized_;
        bool stop_registers_mux_loop_;
        std::thread registers_mux_thread_;
        std::mutex registers_mutex_;
        std::condition_variable registers_condition_variable_;
        Herkus::IHerkusBus& herkus_bus_;
        std::unique_ptr<Regs::IRegisters> registers_;
        std::queue<Common::Registers::RegisterMessage> registers_mux_cache_;
    };
}  // namespace Arina4SoftwareModel::Regs