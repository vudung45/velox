/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "velox/common/base/tests/GTestUtils.h"
#include "velox/functions/prestosql/tests/utils/FunctionBaseTest.h"

namespace facebook::velox::functions::prestosql {
class IPAddressFunctionsTest : public functions::test::FunctionBaseTest {
 protected:
  std::optional<std::string> ipPrefixFunctionFromIpAddress(
      const std::optional<std::string>& input,
      const std::optional<int64_t>& mask) {
    return evaluateOnce<std::string>(
        "cast(ip_prefix(cast(c0 as ipaddress), c1) as varchar)", input, mask);
  }

  std::optional<std::string> ipPrefixFromVarChar(
      const std::optional<std::string>& input,
      const std::optional<int64_t>& mask) {
    return evaluateOnce<std::string>(
        "cast(ip_prefix(c0, c1) as varchar)", input, mask);
  }

  std::optional<std::string> ipSubnetMin(
      const std::optional<std::string>& input) {
    return evaluateOnce<std::string>(
        "cast(ip_subnet_min(cast(c0 as ipprefix)) as varchar)", input);
  }

  std::optional<std::string> ipSubnetMax(
      const std::optional<std::string>& input) {
    return evaluateOnce<std::string>(
        "cast(ip_subnet_max(cast(c0 as ipprefix)) as varchar)", input);
  }
};

TEST_F(IPAddressFunctionsTest, ipPrefixFromIpAddress) {
  ASSERT_EQ(ipPrefixFunctionFromIpAddress("1.2.3.4", 24), "1.2.3.0/24");
  ASSERT_EQ(ipPrefixFunctionFromIpAddress("1.2.3.4", 32), "1.2.3.4/32");
  ASSERT_EQ(ipPrefixFunctionFromIpAddress("1.2.3.4", 0), "0.0.0.0/0");
  ASSERT_EQ(ipPrefixFunctionFromIpAddress("::ffff:1.2.3.4", 24), "1.2.3.0/24");
  ASSERT_EQ(ipPrefixFunctionFromIpAddress("64:ff9b::17", 64), "64:ff9b::/64");
  ASSERT_EQ(
      ipPrefixFunctionFromIpAddress("64:ff9b::17", 127), "64:ff9b::16/127");
  ASSERT_EQ(
      ipPrefixFunctionFromIpAddress("64:ff9b::17", 128), "64:ff9b::17/128");
  ASSERT_EQ(ipPrefixFunctionFromIpAddress("64:ff9b::17", 0), "::/0");
  ASSERT_EQ(
      ipPrefixFunctionFromIpAddress(
          "2001:0db8:85a3:0001:0001:8a2e:0370:7334", 48),
      "2001:db8:85a3::/48");
  ASSERT_EQ(
      ipPrefixFunctionFromIpAddress(
          "2001:0db8:85a3:0001:0001:8a2e:0370:7334", 52),
      "2001:db8:85a3::/52");
  ASSERT_EQ(
      ipPrefixFunctionFromIpAddress(
          "2001:0db8:85a3:0001:0001:8a2e:0370:7334", 128),
      "2001:db8:85a3:1:1:8a2e:370:7334/128");
  ASSERT_EQ(
      ipPrefixFunctionFromIpAddress(
          "2001:0db8:85a3:0001:0001:8a2e:0370:7334", 0),
      "::/0");
  VELOX_ASSERT_THROW(
      ipPrefixFunctionFromIpAddress("::ffff:1.2.3.4", -1),
      "IPv4 subnet size must be in range [0, 32]");
  VELOX_ASSERT_THROW(
      ipPrefixFunctionFromIpAddress("::ffff:1.2.3.4", 33),
      "IPv4 subnet size must be in range [0, 32]");
  VELOX_ASSERT_THROW(
      ipPrefixFunctionFromIpAddress("64:ff9b::10", -1),
      "IPv6 subnet size must be in range [0, 128]");
  VELOX_ASSERT_THROW(
      ipPrefixFunctionFromIpAddress("64:ff9b::10", 129),
      "IPv6 subnet size must be in range [0, 128]");
}

TEST_F(IPAddressFunctionsTest, ipPrefixFromVarChar) {
  ASSERT_EQ(ipPrefixFromVarChar("1.2.3.4", 24), "1.2.3.0/24");
  ASSERT_EQ(ipPrefixFromVarChar("1.2.3.4", 32), "1.2.3.4/32");
  ASSERT_EQ(ipPrefixFromVarChar("1.2.3.4", 0), "0.0.0.0/0");
  ASSERT_EQ(ipPrefixFromVarChar("::ffff:1.2.3.4", 24), "1.2.3.0/24");
  ASSERT_EQ(ipPrefixFromVarChar("64:ff9b::17", 64), "64:ff9b::/64");
  ASSERT_EQ(ipPrefixFromVarChar("64:ff9b::17", 127), "64:ff9b::16/127");
  ASSERT_EQ(ipPrefixFromVarChar("64:ff9b::17", 128), "64:ff9b::17/128");
  ASSERT_EQ(ipPrefixFromVarChar("64:ff9b::17", 0), "::/0");
  VELOX_ASSERT_THROW(
      ipPrefixFromVarChar("::ffff:1.2.3.4", -1),
      "IPv4 subnet size must be in range [0, 32]");
  VELOX_ASSERT_THROW(
      ipPrefixFromVarChar("::ffff:1.2.3.4", 33),
      "IPv4 subnet size must be in range [0, 32]");
  VELOX_ASSERT_THROW(
      ipPrefixFromVarChar("64:ff9b::10", -1),
      "IPv6 subnet size must be in range [0, 128]");
  VELOX_ASSERT_THROW(
      ipPrefixFromVarChar("64:ff9b::10", 129),
      "IPv6 subnet size must be in range [0, 128]");
  VELOX_ASSERT_THROW(
      ipPrefixFromVarChar("localhost", 24),
      "Cannot cast value to IPADDRESS: localhost");
  VELOX_ASSERT_THROW(
      ipPrefixFromVarChar("64::ff9b::10", 24),
      "Cannot cast value to IPADDRESS: 64::ff9b::10");
  VELOX_ASSERT_THROW(
      ipPrefixFromVarChar("64:face:book::10", 24),
      "Cannot cast value to IPADDRESS: 64:face:book::10");
  VELOX_ASSERT_THROW(
      ipPrefixFromVarChar("123.456.789.012", 24),
      "Cannot cast value to IPADDRESS: 123.456.789.012");
}

TEST_F(IPAddressFunctionsTest, ipSubnetMin) {
  ASSERT_EQ(ipSubnetMin("1.2.3.4/24"), "1.2.3.0");
  ASSERT_EQ(ipSubnetMin("1.2.3.4/32"), "1.2.3.4");
  ASSERT_EQ(ipSubnetMin("64:ff9b::17/64"), "64:ff9b::");
  ASSERT_EQ(ipSubnetMin("64:ff9b::17/127"), "64:ff9b::16");
  ASSERT_EQ(ipSubnetMin("64:ff9b::17/128"), "64:ff9b::17");
  ASSERT_EQ(ipSubnetMin("64:ff9b::17/0"), "::");
  ASSERT_EQ(ipSubnetMin("192.64.1.1/9"), "192.0.0.0");
  ASSERT_EQ(ipSubnetMin("192.64.1.1/0"), "0.0.0.0");
  ASSERT_EQ(ipSubnetMin("192.64.1.1/1"), "128.0.0.0");
  ASSERT_EQ(ipSubnetMin("192.64.1.1/31"), "192.64.1.0");
  ASSERT_EQ(ipSubnetMin("192.64.1.1/32"), "192.64.1.1");
  ASSERT_EQ(
      ipSubnetMin("2001:0db8:85a3:0001:0001:8a2e:0370:7334/48"),
      "2001:db8:85a3::");
  ASSERT_EQ(ipSubnetMin("2001:0db8:85a3:0001:0001:8a2e:0370:7334/0"), "::");
  ASSERT_EQ(ipSubnetMin("2001:0db8:85a3:0001:0001:8a2e:0370:7334/1"), "::");
  ASSERT_EQ(
      ipSubnetMin("2001:0db8:85a3:0001:0001:8a2e:0370:7334/127"),
      "2001:db8:85a3:1:1:8a2e:370:7334");
  ASSERT_EQ(
      ipSubnetMin("2001:0db8:85a3:0001:0001:8a2e:0370:7334/128"),
      "2001:db8:85a3:1:1:8a2e:370:7334");
}

TEST_F(IPAddressFunctionsTest, ipSubnetMax) {
  ASSERT_EQ(ipSubnetMax("1.2.3.128/26"), "1.2.3.191");
  ASSERT_EQ(ipSubnetMax("192.168.128.4/32"), "192.168.128.4");
  ASSERT_EQ(ipSubnetMax("10.1.16.3/9"), "10.127.255.255");
  ASSERT_EQ(ipSubnetMax("2001:db8::16/127"), "2001:db8::17");
  ASSERT_EQ(ipSubnetMax("2001:db8::16/128"), "2001:db8::16");
  ASSERT_EQ(ipSubnetMax("64:ff9b::17/64"), "64:ff9b::ffff:ffff:ffff:ffff");
  ASSERT_EQ(ipSubnetMax("64:ff9b::17/72"), "64:ff9b::ff:ffff:ffff:ffff");
  ASSERT_EQ(
      ipSubnetMax("64:ff9b::17/0"), "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff");
  ASSERT_EQ(ipSubnetMax("192.64.1.1/9"), "192.127.255.255");
  ASSERT_EQ(ipSubnetMax("192.64.1.1/0"), "255.255.255.255");
  ASSERT_EQ(ipSubnetMax("192.64.1.1/1"), "255.255.255.255");
  ASSERT_EQ(ipSubnetMax("192.64.1.1/31"), "192.64.1.1");
  ASSERT_EQ(ipSubnetMax("192.64.1.1/32"), "192.64.1.1");
  ASSERT_EQ(
      ipSubnetMax("2001:0db8:85a3:0001:0001:8a2e:0370:7334/48"),
      "2001:db8:85a3:ffff:ffff:ffff:ffff:ffff");
  ASSERT_EQ(
      ipSubnetMax("2001:0db8:85a3:0001:0001:8a2e:0370:7334/0"),
      "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff");
  ASSERT_EQ(
      ipSubnetMax("2001:0db8:85a3:0001:0001:8a2e:0370:7334/1"),
      "7fff:ffff:ffff:ffff:ffff:ffff:ffff:ffff");
  ASSERT_EQ(
      ipSubnetMax("2001:0db8:85a3:0001:0001:8a2e:0370:7334/127"),
      "2001:db8:85a3:1:1:8a2e:370:7335");
  ASSERT_EQ(
      ipSubnetMax("2001:0db8:85a3:0001:0001:8a2e:0370:7334/128"),
      "2001:db8:85a3:1:1:8a2e:370:7334");
}

} // namespace facebook::velox::functions::prestosql
