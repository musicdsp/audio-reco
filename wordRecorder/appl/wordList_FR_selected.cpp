/** @file
 * @author Edouard DUPIN 
 * @copyright 2019, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/ewol.hpp>
#include <appl/debug.hpp>
#include <appl/wordList_FR_selected.hpp>
#include <etk/tool.hpp>

static const char* myWordList[] = {
	"bonjour",
	"ordinateur",
	"oui",
	"non",
	"démare",
	"arrête",
	"joue",
	"suivant",
	"précédent",
	"précédente",
	"heure",
	"jour",
	"moi",
	"année",
	"ne",
	"pas",
	"est",
	"maître",
	"esclave",
	"quelle",
	"quel",
	"comment",
	"pourquoi",
	"zoo",
	"wagon",
	"téléphone",
	"télévision",
	"appel",
	"film",
	"musique",
	"vidéo",
	"annimation",
	"enfant",
	"adulte"
};
static int32_t wordListSize = sizeof(myWordList)/sizeof(char*);

etk::String appl::wordList::getWord_FR_selected(int32_t _id) {
	return myWordList[_id];
}

int32_t appl::wordList::getRandWord_FR_selected() {
	return etk::tool::irand(0, wordListSize-1);
}

int32_t appl::wordList::getWord_FR_selected_count() {
	return wordListSize;
}

