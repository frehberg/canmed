/*
 * utils.h
 *
 *  Created on: Mar 27, 2015
 *      Author: frehberg
 */

#ifndef UTILS_H_
#define UTILS_H_


#include <iostream>
#include <giomm.h>
#include <glibmm.h>
#include <map>
#include "keyvaluetable.h"

Glib::ustring
socket_address_to_string (const Glib::RefPtr<Gio::SocketAddress>& address);

Glib::RefPtr<KeyValueTable>
explode_device_config(Glib::ustring &str);

#endif /* UTILS_H_ */
