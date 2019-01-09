/** @file
 * @author Edouard DUPIN 
 * @copyright 2019, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <elog/log.hpp>

namespace appl {
	int32_t getLogId();
}

#define APPL_BASE(info,data) ELOG_BASE(appl::getLogId(),info,data)

#define APPL_PRINT(data)         APPL_BASE(-1, data)
#define APPL_CRITICAL(data)      APPL_BASE(1, data)
#define APPL_ERROR(data)         APPL_BASE(2, data)
#define APPL_WARNING(data)       APPL_BASE(3, data)
#ifdef DEBUG
	#define APPL_INFO(data)          APPL_BASE(4, data)
	#define APPL_DEBUG(data)         APPL_BASE(5, data)
	#define APPL_VERBOSE(data)       APPL_BASE(6, data)
	#define APPL_TODO(data)          APPL_BASE(4, "TODO : " << data)
#else
	#define APPL_INFO(data)          do { } while(false)
	#define APPL_DEBUG(data)         do { } while(false)
	#define APPL_VERBOSE(data)       do { } while(false)
	#define APPL_TODO(data)          do { } while(false)
#endif

#define APPL_ASSERT(cond,data) \
	do { \
		if (!(cond)) { \
			APPL_CRITICAL(data); \
			assert(!#cond); \
		} \
	} while (0)

