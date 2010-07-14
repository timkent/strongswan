/*
 * Copyright (C) 2010 Martin Willi
 * Copyright (C) 2010 revosec AG
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include "pkcs11_plugin.h"

#include <library.h>
#include <debug.h>
#include <utils/linked_list.h>

#include "pkcs11_library.h"

typedef struct private_pkcs11_plugin_t private_pkcs11_plugin_t;

/**
 * private data of pkcs11_plugin
 */
struct private_pkcs11_plugin_t {

	/**
	 * public functions
	 */
	pkcs11_plugin_t public;

	/**
	 * List of loaded libraries
	 */
	linked_list_t *libs;
};

METHOD(plugin_t, destroy, void,
	private_pkcs11_plugin_t *this)
{
	this->libs->destroy_offset(this->libs, offsetof(pkcs11_library_t, destroy));
	free(this);
}

/*
 * see header file
 */
plugin_t *pkcs11_plugin_create()
{
	private_pkcs11_plugin_t *this;
	enumerator_t *enumerator;
	char *module, *path;

	INIT(this,
		.public.plugin.destroy = _destroy,
		.libs = linked_list_create(),
	);

	enumerator = lib->settings->create_section_enumerator(lib->settings,
										"libstrongswan.plugins.pkcs11.modules");
	while (enumerator->enumerate(enumerator, &module))
	{
		pkcs11_library_t *p11lib;

		path = lib->settings->get_str(lib->settings,
				"libstrongswan.plugins.pkcs11.modules.%s.path", NULL, module);
		if (!path)
		{
			DBG1(DBG_CFG, "PKCS11 module '%s' misses library path", module);
			continue;
		}
		p11lib = pkcs11_library_create(module, path);
		if (p11lib)
		{
			this->libs->insert_last(this->libs, p11lib);
		}
	}
	enumerator->destroy(enumerator);
	return &this->public.plugin;
}
