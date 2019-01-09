/** @file
 * @author Edouard DUPIN 
 * @copyright 2019, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/ewol.hpp>
#include <appl/debug.hpp>
#include <appl/wordList_number.hpp>
#include <etk/tool.hpp>

static int32_t wordListSize = 2000000000;

etk::String appl::wordList::getWord_number(int32_t _id) {
	return etk::toString(_id);
}

int32_t appl::wordList::getRandWord_number() {
	return etk::tool::irand(0, wordListSize-1);
}

int32_t appl::wordList::getWord_number_count() {
	return wordListSize;
}

