/*
 * handler.h
 *
 *  Created on: Mar 24, 2015
 *      Author: frehberg
 */

#ifndef HANDLER_H_
#define HANDLER_H_

#include <iostream>
#include <map>
#include <list>

#include <giomm.h>
#include <glibmm.h>
#include "utils.h"
#include "canmessage.h"
#include "handler_base.h"
#include "appcontext.h"
#include "keyvaluetable.h"

#define GLIBMM_WITH_SIGNALS 1

class UdpHandler : public Handler {
public:
	static Glib::RefPtr<UdpHandler> create(
			Glib::RefPtr<AppContext>& context,
			Glib::RefPtr<KeyValueTable>& config)
	{
		Glib::RefPtr<Gio::SocketAddress> src_address;
		gboolean allow_reuse = true;
		Glib::ustring PARAM_IPV6("ipv6");
		Glib::ustring PARAM_PORT("port");
		Glib::ustring PARAM_GROUP("group");
		// Glib::ustring PARAM_MODE("inout");
		Glib::ustring PARAM_ADDR("addr");
		// Glib::ustring PARAM_DEST("dest");

		gboolean      use_ipv6 = config->get_with_default_as_bool(PARAM_IPV6, false);
		gushort       port     = config->get_with_default_as_int(PARAM_PORT, 7777);
		Glib::ustring group    = config->get_with_default(PARAM_GROUP, "");
		Glib::ustring addr     = config->get_with_default(PARAM_ADDR, "");

		Gio::SocketType socket_type;
		Gio::SocketFamily socket_family;

		socket_type   = Gio::SOCKET_TYPE_DATAGRAM;
		socket_family = use_ipv6 ? Gio::SOCKET_FAMILY_IPV6 : Gio::SOCKET_FAMILY_IPV4;

		Glib::RefPtr<Gio::Socket> socket =
				Gio::Socket::create (
						socket_family,
						socket_type,
						Gio::SOCKET_PROTOCOL_DEFAULT);

        socket->set_blocking (false);
        Glib::RefPtr<Gio::InetAddress> inet_addr = addr.empty()
        		? Gio::InetAddress::create_any (socket_family)
        		:  Gio::InetAddress::create(addr);

		src_address = Gio::InetSocketAddress::create (inet_addr, port);

		std::cout << Glib::ustring::compose ("Binding to %1 [%2]\n",
				socket_address_to_string(src_address),
				allow_reuse);

		socket->bind (src_address, allow_reuse);

		return Glib::RefPtr<UdpHandler>(new UdpHandler(context, socket));
	}


	void
	activate (Glib::IOCondition condition)
	{
		std::cout << "condition " << condition << std::endl;
		if (condition & G_IO_OUT)
		{
			std::cout << "activating G_IO_OUT" << std::endl;

#ifdef GLIBMM_WITH_SIGNALS
			GSource *source;

			source = g_socket_create_source (_socket->gobj (),
					(GIOCondition) condition, NULL);
			g_source_set_callback (source,
					(GSourceFunc) handle_snd_event,
					this, NULL);
			g_source_attach (source, NULL);
#else
			Gio::signal_socket().connect(sigc::ptr_fun(&handle_snd_event), _socket, condition);
#endif
		}

		if (condition & G_IO_IN)
		{
			std::cout << "activating G_IO_IN" << std::endl;

#ifdef GLIBMM_WITH_SIGNALS
			GSource *source;

			source = g_socket_create_source (_socket->gobj (),
					(GIOCondition) condition, NULL);
			g_source_set_callback (source,
					(GSourceFunc) handle_rcv_event,
					this, NULL);
			g_source_attach (source, NULL);
#else
			Gio::signal_socket().connect(sigc::ptr_fun(&handle_rcv_event), _socket, condition);
#endif
		}
	}

	void
	deactivate ()
	{
		_socket->close();
	}

	gboolean handle_rcv_event_cb(GSocket *socket,
			GIOCondition condition) {

		Glib::RefPtr<Gio::SocketAddress> address;

		gchar buffer[4096] = { };

		gssize size =
				_socket->receive_from (address,
						buffer, sizeof buffer);

		Glib::RefPtr<CanMessage> canmessage (new CanMessage());

		std::cerr << Glib::ustring::compose ("rcv [%1]: %2", size, buffer);

		_context->get_handler_pool()->distribute(get_uid(), canmessage);

		if (buffer[0]=='q')
		{
			_context->get_main_loop()->quit();

			return FALSE;
		}

		return TRUE;
	}

	gboolean handle_snd_event_cb(GSocket *socket,
			GIOCondition condition) {

		Glib::RefPtr<Gio::SocketAddress> address;

		//gchar buffer[4096] = "none";

		// std::cerr << Glib::ustring::compose ("snd [%1]: %2", strlen(buffer), buffer);
		std::cerr << "snd " << std::endl;

		Glib::RefPtr<CanMessage> can_mesg = queue_pop();

		// sned the message out
//		gssize size =
//				_socket->send_to(address,
//						buffer, sizeof buffer);

		gboolean continue_snd = ! queue_is_empty();

		return continue_snd;
	}

	virtual ~UdpHandler() {}

private:
	Glib::RefPtr<Gio::Socket> getRecvSocket() { return _socket; };
	Glib::RefPtr<Glib::MainLoop> getLoop() { return _context->get_main_loop(); };

	UdpHandler(Glib::RefPtr<AppContext>& context, Glib::RefPtr<Gio::Socket>& socket)
		: _context(context), _socket(socket)
	{
	}

	/** 
	 * Implementing the following function:
	 * 
	 * gboolean (*GSocketSourceFunc) (GSocket *socket,
     *                				  GIOCondition condition,
     *                	              gpointer user_data);
	 */
	static gboolean handle_snd_event(GSocket *socket,
			GIOCondition condition,
			gpointer user_data)
	{
		UdpHandler *handler = static_cast<UdpHandler*>(user_data);

		return handler->handle_snd_event_cb(socket, condition);
		return FALSE;
	}
	
	/** 
	 * Implementing the following function:
	 * 
	 * gboolean (*GSocketSourceFunc) (GSocket *socket,
     *                				  GIOCondition condition,
     *                	              gpointer user_data);
	 */
	static gboolean handle_rcv_event(GSocket *socket,
			GIOCondition condition,
			gpointer user_data)
	{
		UdpHandler *handler = static_cast<UdpHandler*>(user_data);

		return handler->handle_rcv_event_cb(socket, condition);
	}

	Glib::RefPtr<AppContext>       _context;
	Glib::RefPtr<Gio::Socket>      _socket;
};



#endif /* HANDLER_H_ */
