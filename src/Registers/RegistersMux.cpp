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

#include "Registers/RegistersMux.h"

#include "Common/HerkusBusTopics.h"
#include "spdlog/spdlog.h"

namespace Arina4SoftwareModel::Regs {

    RegistersMux::RegistersMux() : RegistersMux(Herkus::HerkusBus::getInstance(), std::make_unique<Regs::Registers>()) {}

    RegistersMux::RegistersMux(Herkus::IHerkusBus& herkus_bus, std::unique_ptr<Regs::IRegisters> registers)
        : is_initialized_{false},
          stop_registers_mux_loop_{false},
          registers_mux_thread_{},
          registers_mux_cache_mutex_{},
          registers_condition_variable_{},
          herkus_bus_(herkus_bus),
          registers_{std::move(registers)},
          registers_mux_cache_{} {}

    bool RegistersMux::Initialize() {
        spdlog::info("[RegistersMux] Initialize() called...");

        spdlog::info("[RegistersMux] Subscribe to the RegistersMux request topic on the HerkusBus");
        herkus_bus_.Subscribe(Common::HerkusBusTopics::kRamRequestTopic, [this](const std::string& topic, const nlohmann::json& message_payload) {
            spdlog::debug("[RamController] Received message on topic {}: {}", topic, message_payload.dump());
            Common::Registers::RegisterMessage registers_message = message_payload.get<Common::Registers::RegisterMessage>();

            {  // protected by mutex
                std::lock_guard<std::mutex> lock(registers_mux_cache_mutex_);
                registers_mux_cache_.push(registers_message);
            }  // protected by mutex

            spdlog::debug("[RegistersMux] Notifying RegistersMux processing loop about new data byte message");
            registers_condition_variable_.notify_one();
        });

        is_initialized_ = true;
        spdlog::debug("[RegistersMux] is_initialized_ set to true");

        spdlog::info("[RegistersMux] RegistersMux initialized successfully");
        return true;
    }

    bool RegistersMux::StartRegistersMux() {}
    void RegistersMux::StopRegistersMux() {}

    void RegistersMux::RegistersMuxLoop() {
        spdlog::info("[RegistersMux] Starting RAM controller processing loop...");

        while (!stop_registers_mux_loop_) {
            Common::Registers::RegisterMessage register_message;

            {  // protected by mutex
                std::unique_lock<std::mutex> lock(registers_mux_cache_mutex_);
                registers_condition_variable_.wait(lock, [this] { return stop_registers_mux_loop_ || !registers_mux_cache_.empty(); });

                if (stop_registers_mux_loop_) {
                    return;
                }

                register_message = registers_mux_cache_.front();
                registers_mux_cache_.pop();
            }  // protected by mutex

            if (register_message.operation == Common::Registers::RegisterOperation::Write) {
                // register_message.success = registers_->Write(register_message.register_idx, register_message.value);
            }

            if ((register_message.operation == Common::Registers::RegisterOperation::Read)) {
                // std::optional<uint8_t> read_byte = ram_->Read(register_message.register_idx);

                // if (read_byte != std::nullopt) {
                //     register_message.success = true;
                // } else {
                //     register_message.success = false;
                // }
            }

            herkus_bus_.Publish(Common::HerkusBusTopics::kRamReplyTopic, Herkus::json(register_message));
        }
    }

}  // namespace Arina4SoftwareModel::Regs