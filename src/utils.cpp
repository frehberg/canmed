/*
 * utils.cpp
 *
 *  Created on: Mar 27, 2015
 *      Author: frehberg
 */

#include "utils.h"

Glib::ustring
socket_address_to_string (const Glib::RefPtr<Gio::SocketAddress>& address)
{
    Glib::RefPtr<Gio::InetAddress> inet_address;
    Glib::ustring str, res;
    int port;

    Glib::RefPtr<Gio::InetSocketAddress> isockaddr =
        Glib::RefPtr<Gio::InetSocketAddress>::cast_dynamic (address);
    if (!isockaddr)
        return Glib::ustring ();
    inet_address = isockaddr->get_address ();
    str = inet_address->to_string ();
    port = isockaddr->get_port ();
    res = Glib::ustring::compose ("%1:%2", str, port);
    return res;
}

Glib::RefPtr<KeyValueTable> explode_device_config(Glib::ustring &str)
{

	Glib::RefPtr<KeyValueTable> result( new KeyValueTable() );

	int col_idx = str.find(':');

	if (col_idx<0 || col_idx>8) {
		std::cerr << "bad transport id " << str << std::endl;
		return Glib::RefPtr<KeyValueTable>();
	}

	Glib::ustring transportId("transport");
	Glib::ustring transValue  = str.substr(0, col_idx);
	Glib::ustring params = str.substr(col_idx+1);

	std::cerr << "  transport=" << transValue << std::endl;

	result->add(transportId, transValue);

	std::vector<Glib::ustring> params_list =
			Glib::Regex::split_simple(",", params);

	for(guint i=0; i<params_list.size(); i++) {
		Glib::ustring option = params_list.at(i);

		std::vector<Glib::ustring> pair =
				Glib::Regex::split_simple("=", option);

		if (pair.size()!=2) {
			std::cerr << "bad option " << option << std::endl;
			return Glib::RefPtr<KeyValueTable>();
		}
		std::cerr << "  " << pair[0] <<"=" << pair[1] << std::endl;

		result->add(pair[0], pair[1]);
	}

	return result;
}
