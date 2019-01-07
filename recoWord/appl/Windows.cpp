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

appl::Windows::Windows() :
  propertyCount(this, "count",
                     5,
                     "Number of time we restart a record"),
  m_composer(null) {
	addObjectType("appl::Windows");
	propertyTitle.setDirectCheck("River IO viewer");
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
	composition += "		<label name='text-to-say' expand='true' fill='true'>\n";
	composition += "			Text to say ...\n";
	composition += "		</label>\n";
	composition += "	</sizer>\n";
	composition += "	<DataViewer name='displayer' expand='true' fill='true'/>\n";
	composition += "</sizer>\n";
	
	m_composer = ewol::widget::Composer::create();
	if (m_composer == null) {
		APPL_CRITICAL(" An error occured ... in the windows creatrion ...");
		return;
	}
	APPL_ERROR("Composer LOAD   [BEGIN]");
	m_composer->loadFromString(composition);
	setSubWidget(m_composer);
	APPL_ERROR("Composer LOAD   [ END ]");
	subBind(ewol::widget::Button, "bt-record", signalPressed, sharedFromThis(), &appl::Windows::onCallbackRecord);
	subBind(ewol::widget::Button, "bt-reset", signalPressed, sharedFromThis(), &appl::Windows::onCallbackGenerate);
	subBind(appl::widget::DataViewer, "displayer", signalFinished, sharedFromThis(), &appl::Windows::onCallbackFinished);
	next();
}

void appl::Windows::onCallbackFinished() {
	APPL_INFO("Recording is finished");
	ememory::SharedPtr<appl::widget::DataViewer> tmpDisp = ememory::dynamicPointerCast<appl::widget::DataViewer>(getSubObjectNamed("displayer"));
	if (tmpDisp != null) {
		tmpDisp->store("Edouard DUPIN", m_list[m_listPos], "FR_fr");
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

void appl::Windows::onCallbackRecord() {
	APPL_INFO("Start/stop Record of data");
	ememory::SharedPtr<appl::widget::DataViewer> tmpDisp = ememory::dynamicPointerCast<appl::widget::DataViewer>(getSubObjectNamed("displayer"));
	if (tmpDisp != null) {
		tmpDisp->recordToggle();
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
	m_listPos++;
	if (m_listPos >= m_list.size()) {
		m_listPos = 0;
		return;
	}
	configureNewText(m_list[m_listPos]);
}

void appl::Windows::updateCurentLabel() {
	auto elem = ememory::dynamicPointerCast<ewol::widget::Label>(getSubObjectNamed("text-to-say"));
	if (elem == null) {
		return;
	}
	elem->propertyValue.set("[" + etk::toString(m_count+1) + "/" + etk::toString(propertyCount.get()) + "] "+ m_textToSay);
}

void appl::Windows::configureNewText(const etk::String& _text) {
	m_textToSay = _text;
	updateCurentLabel();
	onCallbackGenerate();
}
