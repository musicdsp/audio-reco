/** @file
 * @author Edouard DUPIN 
 * @copyright 2019, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ewol/widget/Windows.hpp>
#include <ewol/widget/Composer.hpp>

namespace appl {
	class Windows : public ewol::widget::Windows {
		public:
			eproperty::Value<int32_t> propertyCount;
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
			etk::String m_textToSay;
			etk::Vector<etk::String> m_list = {
				"",
				"bonjour", /*"ordinateur", "démarre", "arrête", "joue",
				"suivant", "suivante", "précédent", "précédente", "heure", "jour", "moi", "année",
				"oui", "non", "ne", "pas", "est",
				"maître", "esclave",
				"quelle", "quel", "comment", "pourquoi",
				"zoo", "wagon", "téléphone", "télévision", "appel", "film", "musique", "vidéo", "annimation", "enfant", "adulte"*/};
			int32_t m_listPos = 0;
			int32_t m_count = 0;
			void configureNewText(const etk::String& _text);
			void next();
			void updateCurentLabel();
	};
}

