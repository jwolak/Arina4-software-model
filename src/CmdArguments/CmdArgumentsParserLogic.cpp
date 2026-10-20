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
#include "CmdArguments/CmdArgumentsParserLogic.h"

#include <iostream>

#include "spdlog/spdlog.h"
#include "version.h"

namespace Arina4SoftwareModel::Cmd {

    namespace {
        constexpr const char* kInputFileExtension = ".hex";
    }  // namespace

    CmdArgumentsParserLogic::CmdArgumentsParserLogic() {}

    void CmdArgumentsParserLogic::PrintHelp() const {
        spdlog::debug("[CmdArgumentsParserLogic] Printing help information");

        std::cout
            << ("Arina4 - CPU simulator CLI\n"
                "Version: " +
                Arina4SoftwareModel::Version::GetVersionString() +
                "\n"
                "\nUsage:\n"
                "  Arina4 [options]\n"
                "\nOptions:\n"
                "  -h, --help                 Show this help message\n"
                "  -v, --version              Show version information\n"
                "  -d, --debug                Enable debug mode\n"
                "  -i, --input <file>         Input assembly source file\n"
                "\nExample:\n"
                "  Arina4 -i code.hex\n");
    }

    void CmdArgumentsParserLogic::PrintVersionInfo() const {
        std::cout
            << ("Arina4 - CPU simulator CLI\n"
                "Version: " +
                Arina4SoftwareModel::Version::GetVersionString() + "\n");
    }

    bool CmdArgumentsParserLogic::EnableDebugMode() const {
        std::cout << "Enabling debug mode..." << std::endl;
        try {
            spdlog::set_level(spdlog::level::debug);
        } catch (const std::exception& e) {
            std::cerr << "Error enabling debug mode: " << e.what() << std::endl;
            std::cerr << "\033[31mFailed to enable debug mode.\033[0m" << std::endl;
            return false;
        }

        spdlog::debug("[CmdArgumentsParserLogic] Debug mode activated");
        std::cout << "\033[32mDebug mode enabled successfully.\033[0m" << std::endl;
        return true;
    }

    std::optional<std::string> CmdArgumentsParserLogic::GetInputFileName(const char* optarg) const {
        std::cerr << "[CmdArgumentsParserLogic] Retrieving input file name" << std::endl;

        if (optarg) {
            std::cout << "[CmdArgumentsParserLogic] Input file name provided: " << std::string(optarg) << std::endl;
            return std::string(optarg);
        }
        std::cerr << "[ERROR] Input file name argument is null" << std::endl;
        std::cerr << "[CmdArgumentsParserLogic] Input file name argument is null." << std::endl;

        return std::nullopt;
    }
}  // namespace Arina4SoftwareModel::Cmd