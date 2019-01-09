/** @file
 * @author Edouard DUPIN 
 * @copyright 2019, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/ewol.hpp>
#include <appl/debug.hpp>
#include <appl/Windows.hpp>
#include <ewol/widget/Label.hpp>
#include <ewol/widget/Button.hpp>
#include <appl/widget/DataViewer.hpp>
#include <etk/tool.hpp>
#include <appl/wordList_FR.hpp>
#include <appl/wordList_FR_ordered.hpp>
#include <appl/wordList_FR_selected.hpp>
#include <appl/wordList_number.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(appl::Windows);
ETK_DECLARE_TYPE(appl::wordMode);

appl::Windows::Windows() :
  propertyCount(this, "count",
                      5,
                      "Number of time we restart a record"),
  propertyInput(this, "input",
                      "microphone",
                      "play microphone stream",
                      &appl::Windows::inputChangeValue),
  propertyRandom(this, "random",
                       false,
                       "play in random mode"),
  propertyCorpusPath(this, "corpus-path",
                           "USER_DATA:///corpus/" + etk::toString(echrono::Clock::now()),
                           "corpus root path"),
  propertyUserName(this, "user-name",
                         "Unknow",
                         "User name to identify the corpus source"),
  propertyUserBirthYear(this, "user-year",
                              0,
                              "Birth year of the user to distingush the age of the user"),
  propertyWordMode(this, "word-mode",
                         appl::wordMode::wordMode_ALL_WORD,
                         "Word list mode"),
  m_composer(null) {
	addObjectType("appl::Windows");
	propertyTitle.setDirectCheck("River IO viewer");
	// set property list:
	propertyWordMode.add(appl::wordMode::wordMode_ALL_WORD, "ALL_WORD");
	propertyWordMode.add(appl::wordMode::wordMode_ORDERED_WORD, "ORDERED_WORD");
	propertyWordMode.add(appl::wordMode::wordMode_SELECTED_WORD, "SELECTED_WORD");
	propertyWordMode.add(appl::wordMode::wordMode_NUMBER, "NUMBER");
}

void appl::Windows::inputChangeValue() {
	ememory::SharedPtr<appl::widget::DataViewer> tmpDisp = ememory::dynamicPointerCast<appl::widget::DataViewer>(getSubObjectNamed("displayer"));
	if (tmpDisp != null) {
		tmpDisp->propertyInput.set(propertyInput.get());
	}
}

void appl::Windows::init() {
	ewol::widget::Windows::init();
	APPL_ERROR("Load Application");
	etk::String composition = etk::String("");
	composition += "<sizer mode='vert'>\n";
	composition += "	<sizer mode='hori' lock='true' min-size='10,10%'>\n";
	composition += "		<button name='bt-record' expand='false,true' fill='true'>\n";
	composition += "			<label>\n";
	composition += "				Start/Stop record\n";
	composition += "			</label>\n";
	composition += "		</button>\n";
	composition += "		<button name='bt-reset' expand='false,true' fill='true'>\n";
	composition += "			<label>\n";
	composition += "				Reset Currrent Record\n";
	composition += "			</label>\n";
	composition += "		</button>\n";
	composition += "		<button name='bt-next' expand='false,true' fill='true'>\n";
	composition += "			<label>\n";
	composition += "				Next\n";
	composition += "			</label>\n";
	composition += "		</button>\n";
	composition += "	</sizer>\n";
	composition += "	<label name='text-to-say' expand='true' fill='true' font-size='75'>\n";
	composition += "		Text to say ...\n";
	composition += "	</label>\n";
	composition += "	<DataViewer name='displayer' expand='true' fill='true' min-size='100,60%'/>\n";
	composition += "</sizer>\n";
	
	m_composer = ewol::widget::Composer::create();
	if (m_composer == null) {
		APPL_CRITICAL(" An error occured ... in the windows creation ...");
		return;
	}
	APPL_ERROR("Composer LOAD   [BEGIN]");
	m_composer->loadFromString(composition);
	setSubWidget(m_composer);
	APPL_ERROR("Composer LOAD   [ END ]");
	subBind(ewol::widget::Button, "bt-record", signalPressed, sharedFromThis(), &appl::Windows::onCallbackRecord);
	subBind(ewol::widget::Button, "bt-reset", signalPressed, sharedFromThis(), &appl::Windows::onCallbackGenerate);
	subBind(ewol::widget::Button, "bt-next", signalPressed, sharedFromThis(), &appl::Windows::resetCount);
	subBind(ewol::widget::Button, "bt-next", signalPressed, sharedFromThis(), &appl::Windows::next);
	subBind(appl::widget::DataViewer, "displayer", signalFinished, sharedFromThis(), &appl::Windows::onCallbackFinished);

}

void appl::Windows::onCallbackFinished() {
	APPL_INFO("Recording is finished");
	ememory::SharedPtr<appl::widget::DataViewer> tmpDisp = ememory::dynamicPointerCast<appl::widget::DataViewer>(getSubObjectNamed("displayer"));
	if (tmpDisp != null) {
		tmpDisp->store(propertyCorpusPath.get(), propertyUserName.get(), propertyUserBirthYear.get(), m_textToSay, "FR_fr");
		m_count++;
		if (m_count >= propertyCount.get()) {
			m_count = 0;
			next();
		} else {
			updateCurentLabel();
			tmpDisp->recordToggle();
		}
	}
}

void appl::Windows::resetCount() {
	m_count = 0;
}

void appl::Windows::onCallbackRecord() {
	APPL_INFO("Start/stop Record of data");
	ememory::SharedPtr<appl::widget::DataViewer> tmpDisp = ememory::dynamicPointerCast<appl::widget::DataViewer>(getSubObjectNamed("displayer"));
	if (tmpDisp != null) {
		tmpDisp->recordToggle();
	}
}

void appl::Windows::stop() {
	APPL_INFO("stop Record of data");
	ememory::SharedPtr<appl::widget::DataViewer> tmpDisp = ememory::dynamicPointerCast<appl::widget::DataViewer>(getSubObjectNamed("displayer"));
	if (tmpDisp != null) {
		tmpDisp->stop();
	}
}

void appl::Windows::onCallbackGenerate() {
	ememory::SharedPtr<appl::widget::DataViewer> tmpDisp = ememory::dynamicPointerCast<appl::widget::DataViewer>(getSubObjectNamed("displayer"));
	if (tmpDisp != null) {
		tmpDisp->stop();
		tmpDisp->reset();
		tmpDisp->start();
	}
}

void appl::Windows::next() {
	m_total++;
	m_listPos++;
	int32_t id = m_listPos-1;
	if (propertyWordMode.get() == wordMode_ALL_WORD) {
		if (propertyRandom.get() == false) {
			if (id >= appl::wordList::getWord_FR_count()) {
				stop();
			}
		} else {
			id = appl::wordList::getRandWord_FR();
		}
		configureNewText(appl::wordList::getWord_FR(id));
	} else if (propertyWordMode.get() == wordMode_ORDERED_WORD) {
		if (propertyRandom.get() == false) {
			if (id >= appl::wordList::getWord_FR_ordered_count()) {
				stop();
			}
		} else {
			id = appl::wordList::getRandWord_FR_ordered();
		}
		configureNewText(appl::wordList::getWord_FR_ordered(id));
	} else if (propertyWordMode.get() == wordMode_NUMBER) {
		if (propertyRandom.get() == false) {
			if (id >= appl::wordList::getWord_number_count()) {
				stop();
			}
		} else {
			id = appl::wordList::getRandWord_number();
		}
		configureNewText(appl::wordList::getWord_number(id));
	} else if (propertyWordMode.get() == wordMode_SELECTED_WORD) {
		if (propertyRandom.get() == false) {
			if (id >= appl::wordList::getWord_FR_selected_count()) {
				stop();
			}
		} else {
			id = appl::wordList::getRandWord_FR_selected();
		}
		configureNewText(appl::wordList::getWord_FR_selected(id));
	}
}

void appl::Windows::updateCurentLabel() {
	auto elem = ememory::dynamicPointerCast<ewol::widget::Label>(getSubObjectNamed("text-to-say"));
	if (elem == null) {
		return;
	}
	elem->propertyValue.set("[" + etk::toString(m_count+1) + "/" + etk::toString(propertyCount.get()) + "] "+ m_textToSay + "<br/> " + etk::toString(m_listPos) + "  total="+ etk::toString(m_total));
}

void appl::Windows::configureNewText(const etk::String& _text) {
	m_textToSay = _text;
	updateCurentLabel();
	onCallbackGenerate();
}
