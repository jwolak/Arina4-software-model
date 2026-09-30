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

#include "CPU/Cpu.h"

#include "Common/ALU/AluReplyMessage.h"
#include "Common/ALU/AluRequestMessage.h"
#include "Common/HerkusBusTopics.h"
#include "spdlog/spdlog.h"

namespace Arina4SoftwareModel::CPU {
    Cpu::Cpu() : Cpu(Herkus::HerkusBus::getInstance()) {}

    Cpu::Cpu(Herkus::IHerkusBus& herkus_bus)
        : is_initialized_{false},
          stop_cpu_execution_instruction_loop_{false},
          cpu_execute_instruction_thread_{},
          cpu_execute_instruction_mutex_{},
          cpu_execute_instruction_condition_variable_{},
          herkus_bus_{herkus_bus} {}

    Cpu::~Cpu() {}

    bool Cpu::Initialize() {
        spdlog::info("[Cpu] Initialize() called...");

        spdlog::info("Subscribe to the Cpu topic on the HerkusBus");
        herkus_bus_.Subscribe(Common::HerkusBusTopics::kAluTopic, [this](const std::string& topic, const nlohmann::json& message_payload) {
            spdlog::debug("[Cpu] Received message on topic {}: {}", topic, message_payload.dump());
            Common::ALU::AluReplyMessage alu_reply_message = message_payload.get<Common::ALU::AluReplyMessage>();

            {  // protected by mutex
                std::lock_guard<std::mutex> lock(cpu_execute_instruction_mutex_);
                alu_reply_queue_.push(alu_reply_message);
            }  // protected by mutex

            spdlog::debug("[Cpu] Notifying ALU processing loop about new ALU reply message");
            cpu_execute_instruction_condition_variable_.notify_one();
        });

        is_initialized_ = true;
        spdlog::debug("[Cpu] is_initialized_ set to true");

        spdlog::info("[Cpu] Cpu initialized successfully");
        return true;
    }

    bool Cpu::StartCpu() {
        if (!is_initialized_) {
            spdlog::error("[Cpu] Cannot start: not initialized");
            return false;
        }
        spdlog::info("[Cpu] Starting CPU...");
        stop_cpu_execution_instruction_loop_ = false;
        cpu_execute_instruction_thread_ = std::thread(&Cpu::CpuExecuteInstructionLoop, this);

        return true;
    }

    void Cpu::StopCpu() {
        {  // protected by mutex
            std::lock_guard lock(cpu_execute_instruction_mutex_);
            stop_cpu_execution_instruction_loop_ = true;
        }  // protected by mutex

        cpu_execute_instruction_condition_variable_.notify_all();

        if (cpu_execute_instruction_thread_.joinable()) {
            cpu_execute_instruction_thread_.join();
        }
    }

    void Cpu::CpuExecuteInstructionLoop() {
        spdlog::info("[Cpu] Starting CPU instruction execution loop...");
        while (!stop_cpu_execution_instruction_loop_) {
            Common::ALU::AluReplyMessage alu_reply_message{};

            {  // protected by mutex
                std::unique_lock<std::mutex> lock(cpu_execute_instruction_mutex_);
                cpu_execute_instruction_condition_variable_.wait(lock, [this] { return stop_cpu_execution_instruction_loop_ || !alu_reply_queue_.empty(); });

                if (stop_cpu_execution_instruction_loop_) {
                    return;
                }

                alu_reply_message = alu_reply_queue_.front();
                alu_reply_queue_.pop();
            }  // protected by mutex

            // Process ALU reply message

            herkus_bus_.Publish(Common::HerkusBusTopics::kAluTopic, Herkus::json(alu_reply_message));
        }
    }

}  // namespace Arina4SoftwareModel::CPU
