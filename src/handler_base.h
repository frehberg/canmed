/*
 * handler_base.h
 *
 *  Created on: Mar 26, 2015
 *      Author: frehberg
 */

#ifndef HANDLER_BASE_H_
#define HANDLER_BASE_H_


#include <iostream>
#include <map>
#include <list>

#include <giomm.h>
#include <glibmm.h>
#include "utils.h"
#include "canmessage.h"


class Handler : public Glib::Object {
public:
	/**
	 *
	 */
	virtual void activate (Glib::IOCondition condition) = 0;

	/**
	 *
	 */
	virtual void deactivate () = 0;


	/**
	 *
	 */
	void enqueue(Glib::RefPtr<CanMessage>& message, guint origin_uid=G_MAXUINT)
	{

		if (origin_uid!=_unique_id) {
			std::cerr << Glib::ustring::compose ("enqueueing from handler %1 into handler %2\n",
					origin_uid,
					_unique_id);

			bool must_activate_handler = snd_handler_is_off();
			// if empty, there is no event handler active,
			// so the event handler must be activated finally.

			_message_queue.push_back(message);

			if (must_activate_handler)
			{
				activate(Glib::IO_OUT);
			}
		}
	}

	/**
	 *
	 */
	guint get_uid() { return _unique_id; }

	/**
	 *
	 */
	virtual ~Handler() {};

protected:

	/** Constructor
	 *
	 */
	Handler() :  _message_queue(),_unique_id(_counter++) {}

	/**
	 *
	 */
	virtual gboolean snd_handler_is_off() { return queue_is_empty(); }

	/**
	 *
	 */
	gboolean queue_is_empty() { return _message_queue.empty(); }

	/**
	 *
	 */
	Glib::RefPtr<CanMessage> queue_top()
	{
		return _message_queue.front();
	}

	/**
	 *
	 */
	Glib::RefPtr<CanMessage> queue_pop()
	{
		Glib::RefPtr<CanMessage> result = _message_queue.front();
		_message_queue.pop_front();
		return result;
	}

private:
	/** non thread safe shared counter
	 *
	 */
	static guint _counter;

	/**
	 *
	 */
	std::list< Glib::RefPtr<CanMessage> > _message_queue;

	/**
	 *
	 */
	guint _unique_id;
};



class HandlerPool : public Glib::Object {
public:

	virtual void add (Glib::RefPtr<Handler>& new_handler)
	{
		_handler_pool.push_back(new_handler);
	}

	virtual void distribute(guint origin_uid, Glib::RefPtr<CanMessage>& message)
	{
		std::list< Glib::RefPtr<Handler> >::const_iterator iterator;
		for (iterator = _handler_pool.begin(); iterator != _handler_pool.end(); ++iterator) {
			Glib::RefPtr<Handler> handler = *iterator;
			handler->enqueue(message, origin_uid);
		}
	}
private:
	std::list< Glib::RefPtr<Handler> > _handler_pool;

};



#endif /* HANDLER_BASE_H_ */
