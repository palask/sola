// Copyright The SOLA Contributors
//
// Licensed under the MIT License.
// For details on the licensing terms, see the LICENSE file.
// SPDX-License-Identifier: MIT

#ifndef MINHTON_CORE_NODE_INFO_H_
#define MINHTON_CORE_NODE_INFO_H_

#include <string>

#include "minhton/core/network_info.h"
#include "minhton/core/peer_info.h"
#include "minhton/message/serialize.h"

namespace minhton {

enum NodeStatus : uint8_t { kUninit = 0, kRunning = 1, kLeft = 2, kFailed = 3 };

///
/// A NodeInfo is based on a PeerInfo and NetworkInfo. These two classes build the core of a node in
/// our peer2peer network
///
class NodeInfo {
public:
  NodeInfo() = default;

  ///
  /// Constructing an initialized NodeInfo object with the given information.
  ///
  /// \param level valid level of a MINHTON m-ary tree
  /// \param number valid number of a MINHTON m-ary tree
  /// \param fanout valid port of a MINHTON m-ary tree
  ///
  NodeInfo(uint32_t level, uint32_t number, uint16_t fanout);

  ///
  /// Constructing an initialized NodeInfo object with the given information.
  ///
  /// \param level valid level of a MINHTON m-ary tree
  /// \param number valid number of a MINHTON m-ary tree
  /// \param fanout valid port of a MINHTON m-ary tree
  /// \param address valid IPv4 address string
  /// \param uint16_t valid network port
  ///
  NodeInfo(uint32_t level, uint32_t number, uint16_t fanout, const std::string &address,
           uint16_t port);

  /// \returns A PeerInfo, which represents the position (Level/Number) of a peer within the p2p
  /// network
  minhton::PeerInfo getPeerInfo() const;

  ///
  /// Sets the PeerInfo within the NodeInfo object
  ///
  /// \param peer_info valid PeerInfo instance
  ///
  void setPeerInfo(const minhton::PeerInfo &peer_info);

  /// \returns A NetworkInfo, which represents the remote host, by having a IPv4/IPv6 IP address and
  /// a port
  minhton::NetworkInfo getNetworkInfo() const;

  ///
  /// Sets the NetworkInfo within the NodeInfo object
  ///
  /// \param network_info valid PeerInfo instance
  ///
  void setNetworkInfo(const minhton::NetworkInfo &network_info);

  std::string getString() const;

  ///
  /// Returns the current state of the node if the node is properly initialized.
  /// If a node is initialized, it exists.
  /// This method depends on initialized state of the PeerInfo and NetworkInfo
  ///
  /// Typical usage:
  /// \code
  ///   isInitialized();
  /// \endcode
  ///
  /// \returns true if the node is proper initialized, otherwise false
  bool isInitialized() const;

  // -------------- PeerInfo access Helper methods -------------------

  /// Helper method, which sets the position for the internal PeerInfo object
  void setPosition(uint32_t level, uint32_t number);

  /// Helper method, which sets the position for the internal PeerInfo object
  void setPosition(PeerInfo other);

  /// Helper method, which sets the fanout for the internal PeerInfo object
  void setFanout(uint16_t fanout);

  /// \returns the level of PeerInfo
  uint32_t getLevel() const;

  /// \returns the number of PeerInfo
  uint32_t getNumber() const;

  /// \returns the fanout of PeerInfo
  uint16_t getFanout() const;

  /// \returns the status of the node
  NodeStatus getStatus() const;

  /// sets the status of the node
  void setStatus(NodeStatus status);

  /// \returns true if the node consists of an actual known peer, otherwise false
  bool isValidPeer() const;

  // -------------- NetworkInfo access Helper methods -------------------

  /// Helper method, which sets the port for NetworkInfo
  void setPort(uint16_t port);

  /// Helper method, which sets the address for NetworkInfo
  void setAddress(const std::string &address);

  /// \returns the port of NetworkInfo
  uint16_t getPort() const;

  /// \returns the address of NetworkInfo
  std::string getAddress() const;

  /// \returns a unique number representing the ip address of NetworkInfo
  uint32_t getAddressValue() const;

  friend bool operator==(const minhton::NodeInfo &n1, const minhton::NodeInfo &n2);
  friend bool operator!=(const minhton::NodeInfo &n1, const minhton::NodeInfo &n2);
  friend bool operator<(const minhton::NodeInfo &n1, const minhton::NodeInfo &n2);
  friend bool operator<=(const minhton::NodeInfo &n1, const minhton::NodeInfo &n2);
  friend bool operator>(const minhton::NodeInfo &n1, const minhton::NodeInfo &n2);
  friend bool operator>=(const minhton::NodeInfo &n1, const minhton::NodeInfo &n2);

  SERIALIZE(peer_info_, network_info_);

private:
  minhton::PeerInfo peer_info_;
  minhton::NetworkInfo network_info_;

  NodeStatus status_{kUninit};
};

struct NodeInfoHasher {
  std::size_t operator()(const minhton::NodeInfo &node) const {
    using std::hash;
    using std::size_t;

    std::size_t peer_hash = minhton::PeerInfoHasher()(node.getPeerInfo());
    std::size_t network_hash = minhton::NetworkInfoHasher()(node.getNetworkInfo());

    return peer_hash ^ network_hash;
  }
};

}  // namespace minhton
#endif
