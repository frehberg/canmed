/*
 * appcontext.h
 *
 *  Created on: Mar 26, 2015
 *      Author: frehberg
 */

#ifndef APPCONTEXT_H_
#define APPCONTEXT_H_

#include <iostream>
#include <giomm.h>
#include <glibmm.h>
#include "handler_base.h"
#include "utils.h"

class AppContext : public Glib::Object {
public:
	AppContext();

	AppContext(Glib::RefPtr<Glib::MainLoop>& loop) : _loop(loop), _handler_pool(new HandlerPool())
	{
	}
	virtual ~AppContext();

	Glib::RefPtr<HandlerPool>    get_handler_pool() { return _handler_pool; }
	Glib::RefPtr<Glib::MainLoop> get_main_loop()    { return _loop; }
private:

	Glib::RefPtr<Glib::MainLoop> _loop;
	Glib::RefPtr<HandlerPool>    _handler_pool;
};

#endif /* APPCONTEXT_H_ */
