/* $Id$ */

/** @file core/address.cpp Implementation of the address. */

#include "../../stdafx.h"

#ifdef ENABLE_NETWORK

#include "address.h"
#include "config.h"
#include "host.h"
#include "../../string_func.h"

const char *NetworkAddress::GetHostname()
{
	if (this->hostname == NULL) {
		this->hostname = strdup(inet_ntoa(((struct sockaddr_in *)&this->address)->sin_addr));
	}
	return this->hostname;
}

uint32 NetworkAddress::GetIP()
{
	assert(this->address.ss_family == AF_INET);

	if (!this->resolved) {
		((struct sockaddr_in *)&this->address)->sin_addr.s_addr = NetworkResolveHost(this->hostname);
		this->resolved = true;
	}
	return ((struct sockaddr_in *)&this->address)->sin_addr.s_addr;
}

uint16 NetworkAddress::GetPort() const
{
	switch (this->address.ss_family) {
		case AF_INET:
			return ntohs(((struct sockaddr_in *)&this->address)->sin_port);

		default:
			NOT_REACHED();
	}
}

const char *NetworkAddress::GetAddressAsString()
{
	/* 6 = for the : and 5 for the decimal port number */
	static char buf[NETWORK_HOSTNAME_LENGTH + 6];

	seprintf(buf, lastof(buf), "%s:%d", this->GetHostname(), this->GetPort());
	return buf;
}

const sockaddr_storage *NetworkAddress::GetAddress()
{
	if (!this->resolved) this->GetIP();
	return &this->address;
}

#endif /* ENABLE_NETWORK */
