//
// HTTPBasicCredentials.cpp
//
// $Id: //poco/1.3/Net/src/HTTPBasicCredentials.cpp#1 $
//
// Library: Net
// Package: HTTP
// Module:  HTTPBasicCredentials
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/NetException.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/String.h"
#include <sstream>


using Poco::Base64Decoder;
using Poco::Base64Encoder;
using Poco::icompare;


namespace Poco {
namespace Net {


const std::string HTTPBasicCredentials::SCHEME = "Basic";


HTTPBasicCredentials::HTTPBasicCredentials()
{
}

	
HTTPBasicCredentials::HTTPBasicCredentials(const std::string& username, const std::string& password):
	_username(username),
	_password(password)
{
}


HTTPBasicCredentials::HTTPBasicCredentials(const HTTPRequest& request)
{
	static const int eof = std::char_traits<char>::eof();

	std::string scheme;
	std::string info;
	request.getCredentials(scheme, info);
	if (icompare(scheme, SCHEME) == 0)
	{
		std::istringstream istr(info);
		Base64Decoder decoder(istr);
		int ch = decoder.get();
		while (ch != eof && ch != ':')
		{
			_username += (char) ch;
			ch = decoder.get();
		}
		if (ch == ':') ch = decoder.get();
		while (ch != eof)
		{
			_password += (char) ch;
			ch = decoder.get();
		}
	}
	else throw NotAuthenticatedException("Basic authentication expected");
}


HTTPBasicCredentials::~HTTPBasicCredentials()
{
}


void HTTPBasicCredentials::setUsername(const std::string& username)
{
	_username = username;
}
	
	
void HTTPBasicCredentials::setPassword(const std::string& password)
{
	_password = password;
}
	
	
void HTTPBasicCredentials::authenticate(HTTPRequest& request)
{
	std::ostringstream ostr;
	Base64Encoder encoder(ostr);
	encoder << _username << ":" << _password;
	encoder.close();
	request.setCredentials(SCHEME, ostr.str());
}


} } // namespace Poco::Net