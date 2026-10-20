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

#include "CmdArguments/CmdArgumentsParser.h"

#include <getopt.h>

#include <iostream>
#include <memory>

namespace Arina4SoftwareModel::Cmd {
    CmdArgumentsParser::CmdArgumentsParser(int argc, char* argv[]) : CmdArgumentsParser(argc, argv, std::make_unique<CmdArgumentsParserLogic>()) {}

    /* For tests purpose only */
    CmdArgumentsParser::CmdArgumentsParser(int argc, char* argv[], std::unique_ptr<CmdArgumentsParserLogic> cmd_arguments_parser_logic)
        : m_argc(argc), m_argv(argv), cmd_arguments_parser_logic_(std::move(cmd_arguments_parser_logic)) {}

    bool CmdArgumentsParser::Parse(std::shared_ptr<CmdArguments> cmd_arguments) {
        static struct option longopts[] = {
            {"help", no_argument, NULL, 'h'},
            {"version", no_argument, NULL, 'v'},
            {"debug", no_argument, NULL, 'd'},
            {"input", required_argument, NULL, 'i'},
        };

        if (m_argc < 2) {
            std::cerr << "No arguments provided!\n\n";
            cmd_arguments_parser_logic_->PrintHelp();
            return false;
        }

        int opt;
        int longindex;
        bool input_set = false;
        bool help_of_info_shown = false;
        while ((opt = getopt_long(m_argc, m_argv, "hvdi:o:l", longopts, &longindex)) != -1) {
            switch (opt) {
                case 'h':
                    cmd_arguments_parser_logic_->PrintHelp();
                    help_of_info_shown = true;
                    return false;

                case 'v':
                    cmd_arguments_parser_logic_->PrintVersionInfo();
                    help_of_info_shown = true;
                    return false;

                case 'd':
                    cmd_arguments_parser_logic_->EnableDebugMode();
                    std::cout << "Debug mode enabled." << std::endl;
                    break;

                case 'i':
                    cmd_arguments->input_program_file_path = cmd_arguments_parser_logic_->GetInputFileName(optarg);
                    if (cmd_arguments->input_program_file_path == std::nullopt) {
                        std::cerr << "Invalid input file name provided.\n";
                        return false;
                    }
                    std::cout << "Input file name set to: " + *cmd_arguments->input_program_file_path << std::endl;
                    input_set = true;
                    break;

                case '?':
                    std::cerr << "Unknown option or missing argument. Use -h or --help for help.\n";
                    break;

                default:
                    break;
            }
        }

        if ((!input_set) && !help_of_info_shown) {
            std::cerr << "-i <input_file> is required.\n";
            return false;
        }

        std::cout << "Command line arguments parsed successfully." << std::endl;
        return true;
    }
}  // namespace Arina4SoftwareModel::Cmd