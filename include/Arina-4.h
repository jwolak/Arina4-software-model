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

#include <memory>

#include "ArithmeticAndLogicalUnit/ArithmeticAndLogicalUnit.h"
#include "CPU/Cpu.h"
#include "CmdArguments/CmdArguments.h"
#include "Memory/RAM/RamController.h"
#include "Memory/ROM/RomController.h"
#include "ProgramLoader/ProgramLoader.h"

namespace Arina4SoftwareModel::arina4 {
    class Arina4 {
      public:
        explicit Arina4(std::shared_ptr<Cmd::CmdArguments> cmd_arguments);
        ~Arina4();
        bool StartArina4();
        void StopArina4();

      protected:
        explicit Arina4(std::shared_ptr<Cmd::CmdArguments> cmd_arguments, std::unique_ptr<CPU::Cpu> cpu,
                        std::unique_ptr<ArithmeticAndLogicalUnit::ArithmeticAndLogicalUnit> alu, std::unique_ptr<RAM::RamController> ram_controller,
                        std::unique_ptr<ROM::RomController> rom_controller, std::unique_ptr<Loader::ProgramLoader> prorgram_loader);

      private:
        std::shared_ptr<Cmd::CmdArguments> cmd_arguments_;
        std::unique_ptr<CPU::Cpu> cpu_;
        std::unique_ptr<ArithmeticAndLogicalUnit::ArithmeticAndLogicalUnit> alu_;
        std::unique_ptr<RAM::RamController> ram_controller_;
        std::unique_ptr<ROM::RomController> rom_controller_;
        std::unique_ptr<Loader::ProgramLoader> prorgram_loader_;
    };
}  // namespace Arina4SoftwareModel::arina4
