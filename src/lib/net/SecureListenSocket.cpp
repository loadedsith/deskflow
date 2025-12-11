/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2025 Deskflow Developers
 * SPDX-FileCopyrightText: (C) 2015 - 2016 Symless Ltd.
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#include "SecureListenSocket.h"

#include "SecureSocket.h"
#include "arch/Arch.h"
#include "arch/ArchException.h"
#include "base/Log.h"
#include "common/Settings.h"
#include "net/SocketMultiplexer.h"

//
// SecureListenSocket
//

SecureListenSocket::SecureListenSocket(
    IEventQueue *events, SocketMultiplexer *socketMultiplexer, IArchNetwork::AddressFamily family,
    SecurityLevel securityLevel
)
    : TCPListenSocket(events, socketMultiplexer, family),
      m_securityLevel{securityLevel}

{
  // do nothing
}

std::unique_ptr<IDataSocket> SecureListenSocket::accept()
{
  std::unique_ptr<SecureSocket> secureSocket;
  try {
    LOG_IPC("SecureListenSocket: accepting new client connection");
    auto acceptedSocket = ARCH->acceptSocket(socket(), nullptr);
    secureSocket = std::make_unique<SecureSocket>(
        events(), socketMultiplexer(), acceptedSocket, m_securityLevel
    );
    secureSocket->initSsl(true);

    setListeningJob();

    // default location of the TLS cert file in users dir
    const auto certPath = Settings::value(Settings::Security::Certificate).toString();
    LOG_IPC("SecureListenSocket: loading certificate from: %s", qPrintable(certPath));
    if (!secureSocket->loadCertificate(certPath)) {
      LOG_IPC("SecureListenSocket: FAILED to load certificate, rejecting connection");
      return nullptr;
    }
    LOG_IPC("SecureListenSocket: certificate loaded successfully");

    LOG_IPC("SecureListenSocket: starting TLS handshake (secureAccept - async)");
    secureSocket->secureAccept();  // This sets up async TLS handshake via serviceAccept
    LOG_IPC("SecureListenSocket: secureAccept() called, returning socket");

    return secureSocket;
  } catch (ArchNetworkException &) {
    if (secureSocket) {
      setListeningJob();
    }
    return nullptr;
  } catch (std::exception &ex) {
    if (secureSocket) {
      setListeningJob();
    }
    throw ex;
  }
}
