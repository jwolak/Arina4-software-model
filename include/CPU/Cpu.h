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
#include <mutex>
#include <queue>
#include <thread>

#include "Common/ALU/AluRequestMessage.h"
#include "HerkusBus.hpp"

namespace Arina4SoftwareModel::CPU {
    class Cpu {
      public:
        Cpu();
        ~Cpu();
        bool Initialize();
        bool StartCpu();
        void StopCpu();

      protected:
        Cpu(Herkus::IHerkusBus& herkus_bus);
        void CpuExecuteInstructionLoop();

      private:
        bool is_initialized_;
        bool stop_cpu_execution_instruction_loop_;
        std::thread cpu_execute_instruction_thread_;
        std::mutex cpu_execute_instruction_mutex_;
        std::condition_variable cpu_execute_instruction_condition_variable_;
        Herkus::IHerkusBus& herkus_bus_;
        std::queue<Common::ALU::AluRequestMessage> alu_requests_queue_;
    };
}  // namespace Arina4SoftwareModel::CPU