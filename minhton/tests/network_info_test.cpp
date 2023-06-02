// Copyright The SOLA Contributors
//
// Licensed under the MIT License.
// For details on the licensing terms, see the LICENSE file.
// SPDX-License-Identifier: MIT

#include "core/network_info.h"

#include <catch2/catch_test_macros.hpp>
#include <unordered_map>

using namespace minhton;

TEST_CASE("NetworkInfoTest Constructor", "[NetworkInfo][Init]") {
  // empty
  minhton::NetworkInfo test;
  REQUIRE(test.getPort() == 0);
  REQUIRE(test.getAddress() == "");

  // with valid address and port
  minhton::NetworkInfo test_2(IPv4_ADDRESS, IPv4_PORT);
  REQUIRE(test_2.getPort() == IPv4_PORT);
  REQUIRE(test_2.getAddress() == IPv4_ADDRESS);

  // with invalid address
  REQUIRE_THROWS_AS(new minhton::NetworkInfo("", IPv4_PORT), std::invalid_argument);

  // with invalid port
  REQUIRE_THROWS_AS(new minhton::NetworkInfo(IPv4_ADDRESS, 0), std::invalid_argument);

  // with invalid address and port
  REQUIRE_THROWS_AS(new minhton::NetworkInfo("", 0), std::invalid_argument);
}

TEST_CASE("NetworkInfo setPort getPort", "[NetworkInfo][Method][setPort getPort]") {
  minhton::NetworkInfo test;
  uint16_t testValue = 1025;

  REQUIRE_NOTHROW(test.setPort(testValue));
  REQUIRE(test.getPort() == testValue);
  REQUIRE_THROWS_AS(test.setPort(PORT_MAX), std::invalid_argument);
  REQUIRE_THROWS_AS(test.setPort(PORT_MIN - 1), std::invalid_argument);
}

TEST_CASE("NetworkInfo setAddress getAddress",
          "[NetworkInfo][Method][setAddress getAddress][IPv4]") {
  minhton::NetworkInfo test;

  // invalid
  REQUIRE_THROWS_AS(test.setAddress(""), std::invalid_argument);
  REQUIRE_THROWS_AS(test.setAddress("a"), std::invalid_argument);
  REQUIRE_THROWS_AS(test.setAddress("b.c.d.e"), std::invalid_argument);
  REQUIRE_THROWS_AS(test.setAddress("1.2.3"), std::invalid_argument);
  REQUIRE_THROWS_AS(test.setAddress("192.168..1"), std::invalid_argument);
  REQUIRE_THROWS_AS(test.setAddress("192.168.a.1"), std::invalid_argument);
  REQUIRE_THROWS_AS(test.setAddress("192.168.1.1:443"), std::invalid_argument);
  REQUIRE_THROWS_AS(test.setAddress("-171.1.1.1"), std::invalid_argument);
  REQUIRE_THROWS_AS(test.setAddress("-171.1.-1.1"), std::invalid_argument);
  REQUIRE_THROWS_AS(test.setAddress("1:"), std::invalid_argument);
  REQUIRE_THROWS_AS(test.setAddress("1:1"), std::invalid_argument);
  REQUIRE_THROWS_AS(test.setAddress("1:1:1"), std::invalid_argument);
  REQUIRE_THROWS_AS(test.setAddress("1:1:1:1"), std::invalid_argument);
  REQUIRE_THROWS_AS(test.setAddress("1:1:1:1:1"), std::invalid_argument);
  REQUIRE_THROWS_AS(test.setAddress("1:1:1:1:1:1"), std::invalid_argument);
  REQUIRE_THROWS_AS(test.setAddress("1:1:1:1:1:1"), std::invalid_argument);
  REQUIRE_THROWS_AS(test.setAddress("1:1:1:1:1:1:"), std::invalid_argument);
  REQUIRE(test.getAddress() == "");

  // valid
  REQUIRE_NOTHROW(test.setAddress("127.0.0.1"));
  REQUIRE(test.getAddress() == "127.0.0.1");
  REQUIRE_NOTHROW(test.setAddress("192.168.0.1"));
  REQUIRE(test.getAddress() == "192.168.0.1");
  REQUIRE_NOTHROW(test.setAddress("2001:4860:4860::8888"));
  REQUIRE(test.getAddress() == "2001:4860:4860::8888");
  REQUIRE_NOTHROW(test.setAddress("::1"));
  REQUIRE(test.getAddress() == "::1");
  REQUIRE_NOTHROW(test.setAddress("fd12:3456:789f:1234:1234:1234:1234:1234"));
  REQUIRE(test.getAddress() == "fd12:3456:789f:1234:1234:1234:1234:1234");
}

TEST_CASE("NetworkInfo isInitialized", "[NetworkInfo][Method][isInitialized]") {
  minhton::NetworkInfo test;
  REQUIRE_FALSE(test.isInitialized());
  test.setAddress("1.2.3.4");
  REQUIRE_FALSE(test.isInitialized());
  test.setPort(2906);
  REQUIRE(test.isInitialized());

  minhton::NetworkInfo test_2;
  test_2.setPort(2908);
  REQUIRE_FALSE(test_2.isInitialized());
  test_2.setAddress("2.3.4.5");
  REQUIRE(test_2.isInitialized());

  minhton::NetworkInfo test_3("12.3.4.5", 2999);
  REQUIRE(test_3.isInitialized());
}

TEST_CASE("NetworkInfo NetworkInfoHasher", "[NetworkInfo][NetworkInfoHasher]") {
  minhton::NetworkInfo net_1("1.2.3.4", 2000);
  minhton::NetworkInfo net_2("1.2.3.4", 2001);
  minhton::NetworkInfo net_3("1.2.3.5", 2001);

  std::unordered_map<minhton::NetworkInfo, int, NetworkInfoHasher> test_map =
      std::unordered_map<minhton::NetworkInfo, int, NetworkInfoHasher>();
  test_map[net_1] = 1;
  test_map[net_2] = 2;
  test_map[net_3] = 3;

  REQUIRE(test_map[net_1] == 1);
  REQUIRE(test_map[net_2] == 2);
  REQUIRE(test_map[net_3] == 3);

  minhton::NetworkInfoHasher hasher = minhton::NetworkInfoHasher();

  REQUIRE(hasher(net_1) == hasher(net_1));
  REQUIRE(hasher(net_1) != hasher(net_2));
  REQUIRE(hasher(net_1) != hasher(net_3));
  REQUIRE(hasher(net_2) == hasher(net_2));
  REQUIRE(hasher(net_2) != hasher(net_3));
  REQUIRE(hasher(net_3) == hasher(net_3));
}

TEST_CASE("NetworkInfo Equal, Unequal", "[NetworkInfo][Equal, Unequal]") {
  minhton::NetworkInfo p1("1.2.3.4", 2000);
  minhton::NetworkInfo p2("1.2.3.5", 2000);
  minhton::NetworkInfo p3("1.2.3.4", 2100);
  minhton::NetworkInfo p4("1.7.3.4", 2000);
  minhton::NetworkInfo p5("1.2.8.4", 2000);
  minhton::NetworkInfo p6("1.2.3.4", 2070);
  minhton::NetworkInfo p7("1.2.2.4", 2100);
  minhton::NetworkInfo p8("1.2.3.4", 2009);
  minhton::NetworkInfo p9("1.2.3.4", 2004);
  minhton::NetworkInfo p10("1.2.3.4", 2200);

  REQUIRE(p1 == p1);
  REQUIRE(p2 == p2);
  REQUIRE(p3 == p3);
  REQUIRE(p4 == p4);
  REQUIRE(p5 == p5);
  REQUIRE(p6 == p6);
  REQUIRE(p7 == p7);
  REQUIRE(p8 == p8);
  REQUIRE(p9 == p9);
  REQUIRE(p10 == p10);

  REQUIRE(p1 != p2);
  REQUIRE(p1 != p3);
  REQUIRE(p1 != p4);
  REQUIRE(p1 != p5);
  REQUIRE(p1 != p6);
  REQUIRE(p1 != p7);
  REQUIRE(p1 != p8);
  REQUIRE(p1 != p9);
  REQUIRE(p1 != p10);
  REQUIRE(p2 != p3);
  REQUIRE(p2 != p4);
  REQUIRE(p2 != p5);
  REQUIRE(p2 != p6);
  REQUIRE(p2 != p7);
  REQUIRE(p2 != p8);
  REQUIRE(p2 != p9);
  REQUIRE(p2 != p10);
  REQUIRE(p3 != p4);
  REQUIRE(p3 != p5);
  REQUIRE(p3 != p6);
  REQUIRE(p3 != p7);
  REQUIRE(p3 != p8);
  REQUIRE(p3 != p9);
  REQUIRE(p3 != p10);
  REQUIRE(p3 != p5);
  REQUIRE(p4 != p6);
  REQUIRE(p4 != p7);
  REQUIRE(p4 != p8);
  REQUIRE(p4 != p9);
  REQUIRE(p4 != p10);
  REQUIRE(p5 != p6);
  REQUIRE(p5 != p7);
  REQUIRE(p5 != p8);
  REQUIRE(p5 != p9);
  REQUIRE(p5 != p10);
  REQUIRE(p6 != p7);
  REQUIRE(p6 != p8);
  REQUIRE(p6 != p9);
  REQUIRE(p6 != p10);
  REQUIRE(p7 != p8);
  REQUIRE(p7 != p9);
  REQUIRE(p7 != p10);
  REQUIRE(p8 != p9);
  REQUIRE(p8 != p10);
  REQUIRE(p9 != p10);
}
