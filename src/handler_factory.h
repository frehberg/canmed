/*
 * handler_factory.h
 *
 *  Created on: Mar 27, 2015
 *      Author: frehberg
 */

#ifndef HANDLER_FACTORY_H_
#define HANDLER_FACTORY_H_

#include "handler.h"
#include "appcontext.h"

class HandlerFactory {
public:
	static Glib::RefPtr<Handler> create(Glib::RefPtr<AppContext> &app_context,
			Glib::RefPtr<KeyValueTable> &handler_config) {
		auto transport = handler_config->get_with_default("transport", "");
		if (transport == "udp") {
			std::cerr << "Creating UdpHandler" << std::endl;

			return Glib::RefPtr<Handler>(
					UdpHandler::create(app_context, handler_config));
		}

		return Glib::RefPtr<Handler>();
	}
private:
	HandlerFactory();
	virtual ~HandlerFactory();


};

#endif /* HANDLER_FACTORY_H_ */
