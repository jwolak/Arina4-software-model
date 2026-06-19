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

#include <gtest/gtest.h>

#include "ALUExecutorMock.h"
#include "ArithmeticAndLogicalUnit/ArithmeticAndLogicalUnit.h"
#include "Common/ALU/AluReplyMessage.h"
#include "Common/ALU/AluRequestMessage.h"
#include "Common/HerkusBusTopics.h"
#include "MockHerkusBus.h"
#include "spdlog/spdlog.h"

namespace arithmetic_and_logical_unit_test {

    using namespace Arina4SoftwareModel;
    using namespace Arina4SoftwareModel::ArithmeticAndLogicalUnit;
    using namespace Arina4SoftwareModel::ArithmeticAndLogicalUnit::ALUExecutor;
    using namespace Herkus;
    using namespace testing;
    using namespace mocks;

    class ArithmeticAndLogicalUnitTestable : public ArithmeticAndLogicalUnit {
      public:
        ArithmeticAndLogicalUnitTestable(Herkus::IHerkusBus& herkus_bus, std::unique_ptr<ALUExecutor::IALUExecutor> alu_executor)
            : ArithmeticAndLogicalUnit(herkus_bus, std::move(alu_executor)) {}

        using ArithmeticAndLogicalUnit::GetIsInitialized;
        using ArithmeticAndLogicalUnit::Initialize;
    };

    class ArithmeticAndLogicalUnitTest : public ::testing::Test {
      public:
        ArithmeticAndLogicalUnitTest()
            : herkus_bus_mock(new StrictMock<MockHerkusBus>()),
              alu_executor_mock(new StrictMock<ALUExecutorMock>()),
              arithmetic_and_logical_unit{*herkus_bus_mock, std::unique_ptr<ALUExecutor::IALUExecutor>(alu_executor_mock)} {}
        ~ArithmeticAndLogicalUnitTest() { delete herkus_bus_mock; }

        StrictMock<MockHerkusBus>* herkus_bus_mock;
        StrictMock<ALUExecutorMock>* alu_executor_mock;
        ArithmeticAndLogicalUnitTestable arithmetic_and_logical_unit;
    };

    TEST_F(ArithmeticAndLogicalUnitTest, Not_Initialized_And_Inialiazed_Status_As_False_Returned) {
        EXPECT_FALSE(arithmetic_and_logical_unit.GetIsInitialized());
    }

    TEST_F(ArithmeticAndLogicalUnitTest, Initialize_Should_Subscribe_To_ALU_Topic_On_HerkusBus) {
        EXPECT_CALL(*herkus_bus_mock, Subscribe(Common::HerkusBusTopics::kAluTopic, _)).Times(1);
        EXPECT_TRUE(arithmetic_and_logical_unit.Initialize());
    }

    TEST_F(ArithmeticAndLogicalUnitTest, Initialize_Should_Set_IsInitialized_To_True) {
        EXPECT_CALL(*herkus_bus_mock, Subscribe(Common::HerkusBusTopics::kAluTopic, _)).Times(1);
        arithmetic_and_logical_unit.Initialize();
        EXPECT_TRUE(arithmetic_and_logical_unit.GetIsInitialized());
    }

    TEST_F(ArithmeticAndLogicalUnitTest, Initialize_Should_Publish_ALU_Response_On_HerkusBus_When_ALU_Request_Received) {
        Common::ALU::AluRequestMessage alu_request_message{"ADD", 5, 3, 1};
        Common::ALU::AluReplyMessage alu_reply_message{"ADD", 1};

        EXPECT_CALL(*herkus_bus_mock, Subscribe(Common::HerkusBusTopics::kAluTopic, _))
            .WillOnce(Invoke([&](const std::string& topic, Herkus::subscriber_callback sub_callback) {
                nlohmann::json message_payload = alu_request_message;
                sub_callback(topic, message_payload);
            }));

        EXPECT_CALL(*alu_executor_mock, Execute("ADD", 5, 3)).WillOnce(Return(alu_reply_message));
        EXPECT_CALL(*herkus_bus_mock, Publish(Common::HerkusBusTopics::kAluTopic, nlohmann::json(alu_reply_message))).Times(1);

        arithmetic_and_logical_unit.Initialize();
    }

    TEST_F(ArithmeticAndLogicalUnitTest, Initialize_Should_Handle_ALU_Request_Parsing_Error_Gracefully) {
        EXPECT_CALL(*herkus_bus_mock, Subscribe(Common::HerkusBusTopics::kAluTopic, _))
            .WillOnce(Invoke([&](const std::string& topic, Herkus::subscriber_callback sub_callback) {
                nlohmann::json invalid_message_payload = {{"invalid_field", "invalid_value"}};
                sub_callback(topic, invalid_message_payload);
            }));

        EXPECT_CALL(*alu_executor_mock, Execute(_, _, _)).Times(0);
        EXPECT_CALL(*herkus_bus_mock, Publish(_, _)).Times(0);

        arithmetic_and_logical_unit.Initialize();
    }

}  // namespace arithmetic_and_logical_unit_test