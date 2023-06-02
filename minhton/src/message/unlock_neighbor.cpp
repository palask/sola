// Copyright The SOLA Contributors
//
// Licensed under the MIT License.
// For details on the licensing terms, see the LICENSE file.
// SPDX-License-Identifier: MIT

#include "minhton/message/unlock_neighbor.h"

namespace minhton {

MessageUnlockNeighbor::MessageUnlockNeighbor(const MinhtonMessageHeader &header) : header_(header) {
  header_.setMessageType(MessageType::kUnlockNeighbor);
  validate();
}

bool MessageUnlockNeighbor::validateImpl() const { return true; }

}  // namespace minhton
