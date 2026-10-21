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

#include "ProgramLoader/FileHandler.h"

#include "spdlog/spdlog.h"

namespace Arina4SoftwareModel::Loader {

    FileHandler::FileHandler() : file_to_read_{}, file_to_write_{} {}

    FileHandler::~FileHandler() {
        spdlog::trace("[FileHandler] ~FileHandler() called");

        spdlog::debug("[FileHandler] Check is file opened...");
        if (file_to_read_.is_open()) {
            spdlog::debug("[FileHandler] Closing file...");
            file_to_read_.close();
        }
        spdlog::debug("[FileHandler] File closed...");

        if (file_to_write_.is_open()) {
            spdlog::debug("[FileHandler] Closing file to write...");
            file_to_write_.close();
        }
        spdlog::debug("[FileHandler] File to write closed...");
    }

    FileHandler& FileHandler::operator=(FileHandler&& other_file_holder) noexcept {
        spdlog::trace("[FileHandler] Move assignment operator called");

        spdlog::debug("[FileHandler] Check if this is the same object...");
        if (this != &other_file_holder) {
            spdlog::debug("[FileHandler] Check is file opened...");
            if (file_to_read_.is_open()) {
                spdlog::debug("[FileHandler] Closing file...");
                file_to_read_.close();
            }
            spdlog::debug("[FileHandler] Moving file...");
            file_to_read_ = std::move(other_file_holder.file_to_read_);
        }

        spdlog::debug("[FileHandler] Move assignment operator completed");
        return *this;
    }

    bool FileHandler::OpenFileToRead(const std::string& file_path) {
        spdlog::trace("[FileHandler] OpenFileToRead() called with file_path: {}", file_path);

        spdlog::debug("[FileHandler] Attempting to open file...");
        try {
            file_to_read_.open(file_path, std::ios::in | std::ios::binary);
        } catch (const std::exception& e) {
            spdlog::error("[FileHandler] Failed to open file: {}", e.what());
            return false;
        }

        spdlog::debug("[FileHandler] File opened successfully...");
        return file_to_read_.is_open();
    }

    std::ifstream& FileHandler::GetFileToReadStream() {
        spdlog::trace("[FileHandler] GetFileToReadStream() called");
        return file_to_read_;
    }

    bool FileHandler::OpenFileToWrite(const std::string& file_path) {
        spdlog::trace("[FileHandler] OpenFileToWrite() called with file_path: {}", file_path);

        spdlog::debug("[FileHandler] Attempting to open file to write...");
        try {
            file_to_write_.open(file_path, std::ios::out | std::ios::binary);
        } catch (const std::exception& e) {
            spdlog::error("[FileHandler] Failed to open file to write: {}", e.what());
            return false;
        }

        spdlog::debug("[FileHandler] File to write opened successfully...");
        return file_to_write_.is_open();
    }

    std::ofstream& FileHandler::GetFileToWriteStream() {
        spdlog::trace("[FileHandler] GetFileToWriteStream() called");
        return file_to_write_;
    }

}  // namespace Arina4SoftwareModel::Loader