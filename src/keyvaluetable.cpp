/*
 * keyvaluetable.cpp
 *
 *  Created on: Mar 27, 2015
 *      Author: frehberg
 */

#include "keyvaluetable.h"

KeyValueTable::KeyValueTable() : _map() {

}

KeyValueTable::~KeyValueTable() {
	_map.clear();
}

/**
 *
 */
void KeyValueTable::add(Glib::ustring& key, Glib::ustring& value) {
	_map[key] = value;
}

/**
 *
 *  */
Glib::ustring KeyValueTable::get_with_default(Glib::ustring& key, const char* def_value) {
	if (_map.count(key)<1) {
		return Glib::ustring(def_value);
	} else {
		return _map[key];
	}
}

/**
 *
 */
Glib::ustring KeyValueTable::get_with_default(const char* key, const char* def_value)
{
	Glib::ustring tmpkey(key);
	return KeyValueTable::get_with_default(tmpkey, def_value);
}

/**
 *
 */
gboolean KeyValueTable::get_with_default_as_bool(Glib::ustring &key, gboolean def_value)
{
	if (_map.count(key)<1) {
		return def_value;
	} else {
		Glib::ustring &val = _map[key];
		return val == "true" || val.lowercase() == "1";
	}
}

/**
 *
 */
gboolean KeyValueTable::get_with_default_as_bool(const char* key, gboolean def_value)
{
	Glib::ustring tmpkey(key);
	return KeyValueTable::get_with_default_as_bool(tmpkey, def_value);
}


/**
 *
 */
gint KeyValueTable::get_with_default_as_int(Glib::ustring &key, gint def_value)
{
	if (_map.count(key)<1) {
		return def_value;
	} else {
		Glib::ustring &val = _map[key];
		return atoi(val.c_str());
	}
}

/**
 *
 */
gint KeyValueTable::get_with_default_as_int(const char* key, gint def_value)
{
	Glib::ustring tmpkey(key);
	return KeyValueTable::get_with_default_as_bool(tmpkey, def_value);
}

void KeyValueTable::clear() {
	_map.clear();
}
