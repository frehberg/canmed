/*
 * keyvaluetable.h
 *
 *  Created on: Mar 27, 2015
 *      Author: frehberg
 */

#ifndef KEYVALUETABLE_H_
#define KEYVALUETABLE_H_

#include <map>
#include <giomm.h>
#include <glibmm.h>

class KeyValueTable : public Glib::Object  {
public:
	KeyValueTable();
	virtual ~KeyValueTable();

	/**
	 *
	 */
	void add(Glib::ustring& key, Glib::ustring& value);

	/**
	 *
	 */
	Glib::ustring get_with_default(Glib::ustring& key, const char* def_value);
	/**
	 *
	 */
	Glib::ustring get_with_default(const char* key, const char* def_value);


	/**
	 *
	 */
	gboolean get_with_default_as_bool(Glib::ustring &key, gboolean def_val=false);

	/**
	 *
	 */
	gboolean get_with_default_as_bool(const char* key, gboolean def_val=false);

	/**
	 *
	 */
	gint get_with_default_as_int(Glib::ustring &key, gint def_value);

	/**
	 *
	 */
	gint get_with_default_as_int(const char* key, gint def_value);

	/**
	 *
	 */
	void clear();
private:
	/**
	 *
	 */
	std::map<Glib::ustring, Glib::ustring> _map;
};

#endif /* KEYVALUETABLE_H_ */
