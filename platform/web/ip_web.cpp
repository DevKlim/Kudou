#include "ip_web.h"

void IPWeb::_resolve_hostname(List<IPAddress> &r_addresses, const String &p_hostname, Type p_type) const {
}

void IPWeb::get_local_interfaces(HashMap<String, Interface_Info> *r_interfaces) const {
}

void IPWeb::make_default() {
	_create = _create_web;
}

IP *IPWeb::_create_web() {
	return memnew(IPWeb);
}

IPWeb::IPWeb() {
}
