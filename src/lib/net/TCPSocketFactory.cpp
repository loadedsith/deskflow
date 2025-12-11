/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2025 Deskflow Developers
 * SPDX-FileCopyrightText: (C) 2012 - 2016 Symless Ltd.
 * SPDX-FileCopyrightText: (C) 2002 Chris Schoeneman
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#include "net/TCPSocketFactory.h"
#include "net/SecureListenSocket.h"
#include "net/SecureSocket.h"
#include "net/TCPListenSocket.h"
#include "net/TCPSocket.h"
#include "base/Log.h"

//
// TCPSocketFactory
//

TCPSocketFactory::TCPSocketFactory(IEventQueue *events, SocketMultiplexer *socketMultiplexer)
    : m_events(events),
      m_socketMultiplexer(socketMultiplexer)
{
  // do nothing
}

IDataSocket *TCPSocketFactory::create(IArchNetwork::AddressFamily family, SecurityLevel securityLevel) const
{
  const char* levelStr = (securityLevel == SecurityLevel::PlainText) ? "PlainText" :
                         (securityLevel == SecurityLevel::Encrypted) ? "Encrypted" :
                         (securityLevel == SecurityLevel::PeerAuth) ? "PeerAuth" : "Unknown";
  LOG_IPC("TCPSocketFactory::create: SecurityLevel=%s", levelStr);
  
  if (securityLevel != SecurityLevel::PlainText) {
    LOG_IPC("TCPSocketFactory::create: Creating SecureSocket (TLS enabled)");
    auto *secureSocket = new SecureSocket(m_events, m_socketMultiplexer, family, securityLevel);
    secureSocket->initSsl(false);
    return secureSocket;
  } else {
    LOG_IPC("TCPSocketFactory::create: Creating TCPSocket (TLS DISABLED - PlainText)");
    return new TCPSocket(m_events, m_socketMultiplexer, family);
  }
}

IListenSocket *TCPSocketFactory::createListen(IArchNetwork::AddressFamily family, SecurityLevel securityLevel) const
{
  IListenSocket *socket = nullptr;
  if (securityLevel != SecurityLevel::PlainText) {
    socket = new SecureListenSocket(m_events, m_socketMultiplexer, family, securityLevel);
  } else {
    socket = new TCPListenSocket(m_events, m_socketMultiplexer, family);
  }

  return socket;
}
