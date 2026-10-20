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

#include "Memory/RAM/RamController.h"

#include "Common/HerkusBusTopics.h"
#include "Common/Memory/CommonMemory.h"
#include "spdlog/spdlog.h"

namespace Arina4SoftwareModel::RAM {
    RamController::RamController() : RamController(Herkus::HerkusBus::getInstance(), std::make_unique<Ram>(Common::Memory::kRamSize)) {}

    RamController::RamController(Herkus::IHerkusBus& herkus_bus, std::unique_ptr<Ram> ram)
        : is_initialized_{false},
          stop_ram_controller_loop_{false},
          ram_controller_thread_{},
          ram_cache_mutex_{},
          ram_cache_condition_variable_{},
          herkus_bus_(herkus_bus),
          ram_cache_{},
          ram_(std::move(ram)) {}

    bool RamController::Initialize() {
        spdlog::info("[RamController] Initialize() called...");

        spdlog::info("[RamController] Subscribe to the RAM request topic on the HerkusBus");
        herkus_bus_.Subscribe(Common::HerkusBusTopics::kRamRequestTopic, [this](const std::string& topic, const nlohmann::json& message_payload) {
            spdlog::debug("[RamController] Received message on topic {}: {}", topic, message_payload.dump());
            Common::Memory::DataByteMessage data_byte_message = message_payload.get<Common::Memory::DataByteMessage>();

            {  // protected by mutex
                std::lock_guard<std::mutex> lock(ram_cache_mutex_);
                ram_cache_.push(data_byte_message);
            }  // protected by mutex

            spdlog::debug("[RamController] Notifying RAM processing loop about new data byte message");
            ram_cache_condition_variable_.notify_one();
        });

        is_initialized_ = true;
        spdlog::debug("[RamController] is_initialized_ set to true");

        spdlog::info("[RamController] RamController initialized successfully");
        return true;
    }

    bool RamController::StartRamController() {
        if (!is_initialized_) {
            spdlog::error("[RamController] Cannot start: not initialized");
            return false;
        }

        spdlog::info("[RamController] Starting Arithmetic and Logical Unit...");
        stop_ram_controller_loop_ = false;
        ram_controller_thread_ = std::thread(&RamController::RamControllerLoop, this);
        return true;
    }

    void RamController::StopRamController() {
        spdlog::info("[RamController] Stopping RAM Controller and Logical Unit...");
        {
            std::lock_guard<std::mutex> lock(ram_cache_mutex_);
            stop_ram_controller_loop_ = true;
        }

        ram_cache_condition_variable_.notify_one();
        if (ram_controller_thread_.joinable()) {
            ram_controller_thread_.join();
        }
    }

    void RamController::RamControllerLoop() {
        spdlog::info("[RamController] Starting RAM controller processing loop...");

        while (!stop_ram_controller_loop_) {
            Common::Memory::DataByteMessage data_byte_message;

            {  // protected by mutex
                std::unique_lock<std::mutex> lock(ram_cache_mutex_);
                ram_cache_condition_variable_.wait(lock, [this] { return stop_ram_controller_loop_ || !ram_cache_.empty(); });

                if (stop_ram_controller_loop_) {
                    return;
                }

                data_byte_message = ram_cache_.front();
                ram_cache_.pop();
            }  // protected by mutex

            if (data_byte_message.operation == Common::Memory::MemoryOperation::Write) {
                data_byte_message.success = ram_->Write(data_byte_message.offset, data_byte_message.value);
            }

            if ((data_byte_message.operation == Common::Memory::MemoryOperation::Read)) {
                std::optional<uint8_t> read_byte = ram_->Read(data_byte_message.offset);

                if (read_byte != std::nullopt) {
                    data_byte_message.success = true;
                } else {
                    data_byte_message.success = false;
                }
            }

            herkus_bus_.Publish(Common::HerkusBusTopics::kRamReplyTopic, Herkus::json(data_byte_message));
        }
    }

}  // namespace Arina4SoftwareModel::RAM