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

#include "ProgramLoader/ProgramLoader.h"

#include <memory>

#include "ProgramLoader/FileHandler.h"
#include "spdlog/spdlog.h"

namespace Arina4SoftwareModel::Loader {

    ProgramLoader::ProgramLoader() : ProgramLoader(std::make_unique<FileHandler>()) {}

    ProgramLoader::ProgramLoader(std::unique_ptr<IFileHandler> file_handler) : file_handler_{std::move(file_handler)} {}

    bool ProgramLoader::LoadProgram(const std::string& program_path) {
        if (program_path.empty()) {
            spdlog::error("[ProgramLoader] File path is empty");
            return false;
        }

        if (!file_handler_->OpenFileToRead(program_path)) {
            spdlog::error("[ProgramLoader] Failed to open the file: {}", program_path);
            return false;
        }

        spdlog::info("[ProgramLoader] Opened file: {}", program_path);
        return true;
    }

}  // namespace Arina4SoftwareModel::Loader