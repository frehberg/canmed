/*
 * canmessage.h
 *
 *  Created on: Mar 26, 2015
 *      Author: frehberg
 */

#ifndef CANMESSAGE_H_
#define CANMESSAGE_H_

#include <iostream>
#include <giomm.h>
#include <glibmm.h>

class CanMessage : public Glib::Object {
public:
	CanMessage();
	virtual ~CanMessage();

private:

};

#endif /* CANMESSAGE_H_ */
