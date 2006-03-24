/**
 * @file logger_manager.h
 *
 * @brief Interface of logger_manager_t.
 *
 */

/*
 * Copyright (C) 2005 Jan Hutter, Martin Willi
 * Hochschule fuer Technik Rapperswil
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

#ifndef LOGGER_MANAGER_H_
#define LOGGER_MANAGER_H_

#include <pthread.h>

#include <utils/logger.h>


typedef enum logger_context_t logger_context_t;

/**
 * @brief Context of a specific logger.
 * 
 * @ingroup utils
 */
enum logger_context_t {
	PARSER = 0,
	GENERATOR,
	IKE_SA,
	IKE_SA_MANAGER,
	CHILD_SA,
	MESSAGE,
	THREAD_POOL,
	WORKER,
	SCHEDULER,
	SENDER,
	RECEIVER,
	SOCKET,
	TESTER,
	DAEMON,
	CONFIG,
	ENCRYPTION_PAYLOAD,
	PAYLOAD,
	DER_DECODER,
	DER_ENCODER,
	LOGGER_CONTEXT_ROOF,
};


typedef struct logger_manager_t logger_manager_t;

/**
 * @brief Class to manage logger_t objects.
 * 
 * The logger manager manages all logger_t object in a list and
 * allows their manipulation. Via a logger_context_t, the loglevel
 * of a specific logging type can be adjusted at runtime.
 * 
 * @b Constructors:
 *  - logger_manager_create()
 * 
 * @see logger_t
 * 
 * @ingroup utils
 */
struct logger_manager_t {
	
	/**
	 * @brief Gets a logger_t object for a specific logger context.
	 *
	 * @param this			logger_manager_t object
	 * @param context		logger_context to use the logger for
	 * @param name			name for the new logger. Context name is already included 
	 * 						and has not to be specified (so NULL is allowed)
	 * @return				logger_t object
	 */
	logger_t *(*get_logger) (logger_manager_t *this, logger_context_t context);
	
	/**
	 * Returns the set log_level of a specific context or 0.
	 * 
	 * @param this 			calling object
	 * @param context 		context to check level
	 * @return				log_level for the given logger_context
	 */
	log_level_t (*get_log_level) (logger_manager_t *this, logger_context_t context);
	
	/**
	 * Enables a logger level of a specific context.
	 * 
	 * @param this 			calling object
	 * @param context 		context to set level
 	 * @param log_level 	logger level to eanble
	 */
	void (*enable_log_level) (logger_manager_t *this, logger_context_t context,log_level_t log_level);
		

	/**
	 * Disables a logger level of a specific context.
	 * 
	 * @param this 			calling object
	 * @param context 		context to set level
 	 * @param log_level 	logger level to disable
	 */
	void (*disable_log_level) (logger_manager_t *this, logger_context_t context,log_level_t log_level);


	/**
	 * @brief Destroys a logger_manager_t object.
	 * 
	 * All remaining managed logger_t objects are also destroyed.
	 *
	 * @param this		logger_manager_t object
	 */
	void (*destroy) (logger_manager_t *this);
};

/**
 * @brief Constructor to create a logger_manager_t object.
 *
 * @param default_log_level 	default log level for all context
 * @return 						logger_manager_t object
 * 
 * @ingroup utils
 */
logger_manager_t *logger_manager_create(log_level_t default_log_level);


#endif /*LOGGER_MANAGER_H_*/
