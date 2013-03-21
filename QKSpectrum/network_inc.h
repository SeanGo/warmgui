#ifndef __newwork_include_h_include__
#define __newwork_include_h_include__

#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/NetSSL.h>
#include <Poco/Net/X509Certificate.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/SecureServerSocket.h>
#include <Poco/Net/TCPServer.h>
#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/TCPServerConnectionFactory.h>
#include <Poco/Net/TCPServerParams.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SecureStreamSocket.h>
#include <Poco/Thread.h>
#include <Poco/Timespan.h>
#include <Poco/Timestamp.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/Exception.h>
#include <Poco/Stopwatch.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/PropertyFileConfiguration.h>
#include <Poco/Net/ServerConnector.h>
#include <Poco/Net/SocketReactor.h>
#include <Poco/File.h>

#include "clicomm.h"
#include "ClientServiceHandler.h"
#include "NetClient.h"

using namespace AMERICA;



#endif //__newwork_include_h_include__