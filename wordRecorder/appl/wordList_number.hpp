/** @file
 * @author Edouard DUPIN 
 * @copyright 2019, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once
#include <etk/types.hpp>

namespace appl {
	namespace wordList {
		etk::String getWord_number(int32_t _id);
		int32_t getRandWord_number();
		int32_t getWord_number_count();
	};
}
