#include <iostream>
#include <giomm.h>
#include <glibmm.h>
#include "handler.h"
#include "utils.h"
#include "appcontext.h"
namespace
{


Glib::RefPtr<Glib::MainLoop> loop;

std::vector<Glib::ustring,std::allocator<Glib::ustring> > op_devices;
bool op_verbose = false;


/**
 *
 */
class ServerOptionGroup : public Glib::OptionGroup
{
public:
	ServerOptionGroup()
	: Glib::OptionGroup("canmed", "", "")
	{
		Glib::OptionEntry entry;
		entry.set_long_name("device");
		entry.set_short_name('d');
		entry.set_description("Local port to bind to, default: 7777");
		add_entry(entry, op_devices);

		entry.set_long_name("verbose");
		entry.set_short_name('v');
		entry.set_description("Be verbose");
		add_entry(entry, op_verbose);
	}
};  


} // end anonymous namespace

int
main (int argc,
		char *argv[])
{
	// Glib::RefPtr<Gio::SocketAddress> address;

	Gio::init ();

	Glib::OptionContext option_context(" - Test server stuff");
	ServerOptionGroup option_group;
	option_context.set_main_group(option_group);
	try
	{
		option_context.parse(argc, argv);
	}
	catch (const Glib::Error& error)
	{
		std::cerr << Glib::ustring::compose ("%1: %2\n", argv[0], error.what());
		return 1;
	}

	loop = Glib::MainLoop::create ();

	Glib::RefPtr<AppContext> app_context(new AppContext(loop));

	try {
		std::vector<Glib::ustring,std::allocator<Glib::ustring> >::const_iterator iterator;
		for (iterator = op_devices.begin(); iterator != op_devices.end(); ++iterator) {
			Glib::ustring param_str = *iterator;

			Glib::RefPtr<KeyValueTable> keyvals (explode_device_config(param_str));

			Glib::RefPtr<Handler> handler;

			handler = UdpHandler::create(app_context, keyvals);
			handler->activate(Glib::IO_IN);
			app_context->get_handler_pool()->add(handler);
		}
	} catch (const Gio::Error& error)
	{
		std::cerr << Glib::ustring::compose ("%1: %2\n", argv[0], error.what ());
		return 1;
	}
	std::cout << "Entering main loop\n";

	loop->run(); // blocking until termination

	std::cout << "Leaving main loop\n";

	// terminating the app
	// app_context->get_handler_pool()->

	return 0;
}
