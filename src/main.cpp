
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

#include <stdlib.h>

#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <memory>
#include <thread>

#include "Arina-4.h"
#include "CmdArguments/CmdArguments.h"
#include "CmdArguments/CmdArgumentsParser.h"

namespace {
    std::atomic<int> g_signal{0};

    extern "C" void SignalHandler(int sig) { g_signal.store(sig); }

    void InstallSignalHandlers() {
        struct sigaction sa{};
        sa.sa_handler = SignalHandler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;

        sigaction(SIGINT, &sa, nullptr);
        sigaction(SIGTERM, &sa, nullptr);
        sigaction(SIGHUP, &sa, nullptr);
    }
}  // namespace

int main(int argc, char* argv[]) {
    std::cout << "\n\n***** Welcome to Arina4 CPU Software Model! *****\n\n" << std::endl;

    std::shared_ptr<::Arina4SoftwareModel::Cmd::CmdArguments> cmd_arguments = std::make_shared<::Arina4SoftwareModel::Cmd::CmdArguments>();
    Arina4SoftwareModel::Cmd::CmdArgumentsParser cmd_arguments_parser(argc, argv);

    if (!cmd_arguments_parser.Parse(cmd_arguments)) {
        std::cerr << "[Arina4SoftwareModel]: Parse input arguments failed" << std::endl;
        return EXIT_FAILURE;
    }

    InstallSignalHandlers();

    Arina4SoftwareModel::arina4::Arina4 arina4_instance(cmd_arguments);

    if (!arina4_instance.StartArina4()) {
        std::cerr << "[Arina4SoftwareModel]:Failed to start Arina4" << std::endl;
        arina4_instance.StopArina4();
        return EXIT_FAILURE;
    }

    while (g_signal.load() == 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "[Arina4SoftwareModel]: Signal " << g_signal.load() << " caught, stopping Arina4" << std::endl;
    arina4_instance.StopArina4();

    return EXIT_SUCCESS;
}
