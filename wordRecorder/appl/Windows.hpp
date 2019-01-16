/** @file
 * @author Edouard DUPIN 
 * @copyright 2019, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ewol/widget/Windows.hpp>
#include <ewol/widget/Composer.hpp>

namespace appl {
	enum wordMode {
		wordMode_ALL_WORD,
		wordMode_ORDERED_WORD,
		wordMode_NUMBER,
		wordMode_SELECTED_WORD,
	};
	class Windows : public ewol::widget::Windows {
		public:
			eproperty::Value<int32_t> propertyCount;
			eproperty::Value<etk::String> propertyInput;
			eproperty::Value<bool> propertyRandom;
			eproperty::Value<etk::Uri> propertyCorpusPath;
			eproperty::Value<etk::String> propertyUserName;
			eproperty::Value<int32_t> propertyUserBirthYear;
			eproperty::List<enum wordMode> propertyWordMode;
		private:
			ememory::SharedPtr<ewol::widget::Composer> m_composer;
		protected:
			Windows();
			void init() override;
		public:
			DECLARE_FACTORY(Windows);
		public: // callback functions
			void onCallbackRecord();
			void onCallbackGenerate();
			void onCallbackFinished();
			void resetCount();
			void stop();
			void remove();
			etk::String m_textToSay;
			int32_t m_listPos = 0;
			int32_t m_count = 0;
			int32_t m_total = 0;
			void configureNewText(const etk::String& _text);
			void next();
			void updateCurentLabel();
			void inputChangeValue();
			etk::Vector<etk::Uri> m_previousFiles;
	};
}

