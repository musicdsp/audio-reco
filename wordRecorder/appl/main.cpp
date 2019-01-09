/** @file
 * @author Edouard DUPIN 
 * @copyright 2019, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */


#include <etk/types.hpp>
#include <ewol/ewol.hpp>
#include <gale/context/commandLine.hpp>

#include <appl/debug.hpp>
#include <appl/Windows.hpp>
#include <ewol/object/Object.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/context/Context.hpp>
#include <appl/widget/DataViewer.hpp>
#include <etk/theme/theme.hpp>

static const etk::String configurationRiver =
	"{\n"
	"	microphone:{\n"
	"		io:'input',\n"
	"		map-on:{\n"
	"			interface:'auto',\n"
	"			name:'default',\n"
	"		},\n"
	"		frequency:48000,\n"
	"		channel-map:['front-left', 'front-right'],\n"
	"		type:'auto',\n"
	"		nb-chunk:1024\n"
	"	},\n"
	"	microphone_USB:{\n"
	"		io:'input',\n"
	"		map-on:{\n"
	"			interface:'alsa',\n"
	"			name:'hw:1,0',\n"
	"		},\n"
	"		frequency:48000,\n"
	"		channel-map:['front-center'],\n"
	"		type:'int16',\n"
	"		nb-chunk:1024\n"
	"	}\n"
	"}\n";

static void usage() {
	APPL_PRINT("  --random/-r             Random record words (instead of ordered)");
	APPL_PRINT("  --input=XXX             Audio source");
	APPL_PRINT("                              microphone       default Io of the backend (default)");
	APPL_PRINT("                              microphone_USB   alsa hw1,0 input");
	APPL_PRINT("  --source_type=XXX       Record specific word source");
	APPL_PRINT("                              ALL_WORD       All french word availlable (default)");
	APPL_PRINT("                              ORDERED_WORD   The 1500 more used word");
	APPL_PRINT("                              NUMBER         Say mumber -5 to 100 and 1k 1M 1B 1T ...");
	APPL_PRINT("                              SELECTED_WORD  small list of selected word");;
	APPL_PRINT("  --user_name=\"XXX\"     Name of the user");
	APPL_PRINT("  --user_birth_year=XXX   Birth year of the speaker");
	APPL_PRINT("  --corpus_root_path=XXX  Root path of the corpus (add after a folder with the name and other informations");
	exit(0);
}

class MainApplication : public ewol::context::Application {
	public:
		virtual void onCreate(ewol::Context& _context) override {
			APPL_INFO(" == > CREATE ... (START) [" << gale::getBoardType() << "] (" << gale::getCompilationMode() << ") (BEGIN)");
			for( int32_t iii=0 ; iii<_context.getCmd().size(); iii++) {
				etk::String tmpppp = _context.getCmd().get(iii);
				if (    tmpppp == "-h"
				     || tmpppp == "--help") {
					usage();
				}
			}
			etk::theme::setName("COLOR", "color/white/");
			
			_context.setSize(vec2(900, 600));
			
			_context.setTitle("audio-reco-wordRecorder");
			
			// select internal data for font ...
			_context.getFontDefault().setUseExternal(true);
			#ifdef __TARGET_OS__Android
				_context.getFontDefault().set("FreeSerif", 19);
			#else
				_context.getFontDefault().set("FreeSerif;DejaVuSansMono",14);
			#endif
			
			// set the application icon ...
			//_context.setIcon("DATA:///icon.png");
			APPL_INFO("==> CREATE ... (END)");
		}
		
		void onStart(ewol::Context& _context) override {
			APPL_INFO("==> START ... (BEGIN)");
			// init internal global value
			audio::river::initString(configurationRiver);
			
			_context.setSize(vec2(800, 600));
			
			// select internal data for font ...
			_context.getFontDefault().setUseExternal(true);
			_context.getFontDefault().setSize(19);
			
			appl::widget::DataViewer::createManagerWidget(_context.getWidgetManager());
			
			ememory::SharedPtr<appl::Windows> basicWindows = appl::Windows::create();
			if (basicWindows == null) {
				APPL_ERROR("Can not allocate the basic windows");
				_context.exit(-1);
				return;
			}
			// create the specific windows
			_context.setWindows(basicWindows);
			
			// add files
			APPL_INFO("show list of files : ");
			for( int32_t iii=0 ; iii<_context.getCmd().size(); iii++) {
				etk::String tmpppp = _context.getCmd().get(iii);
				if (    tmpppp == "-h"
				     || tmpppp == "--help") {
					// nothing to do ...
				} else if (    tmpppp == "--random"
				            || tmpppp == "-r") {
					// random display word
					APPL_ERROR("SET mode RANDOM");
					basicWindows->propertyRandom.set(true);
				} else if (tmpppp.startWith("--source_type=") == true) {
					etk::String mode = tmpppp.extract(14);
					if (mode == "ALL_WORD") {
						basicWindows->propertyWordMode.set(appl::wordMode_ALL_WORD);
					} else if (mode == "ORDERED_WORD") {
						basicWindows->propertyWordMode.set(appl::wordMode_ORDERED_WORD);
					} else if (mode == "NUMBER") {
						basicWindows->propertyWordMode.set(appl::wordMode_NUMBER);
					} else if (mode == "SELECTED_WORD") {
						basicWindows->propertyWordMode.set(appl::wordMode_SELECTED_WORD);
					} else {
						APPL_ERROR("Can not select mode in list: '" << mode << "'" );
						usage();
					}
				} else if (tmpppp.startWith("--input=") == true) {
					etk::String input = tmpppp.extract(8);
					basicWindows->propertyInput.set(input);
				} else if (tmpppp.startWith("--user_name=") == true) {
					etk::String userName = tmpppp.extract(12);
					basicWindows->propertyUserName.set(userName);
				} else if (tmpppp.startWith("--user_birth_year=") == true) {
					etk::String userBirthDate = tmpppp.extract(18);
					basicWindows->propertyUserBirthYear.set(etk::string_to_int32_t(userBirthDate));
				} else if (tmpppp.startWith("--corpus_root_path=") == true) {
					etk::Path corpusPath = tmpppp.extract(19);
					basicWindows->propertyCorpusPath.set(corpusPath);
				} else {
					APPL_ERROR("Unknow parameter: '" << tmpppp << "'" );
					//usage();
				}
			}
			basicWindows->next();
			APPL_INFO("==> START ... (END)");
			return;
		}
		void onStop(ewol::Context& _context) override {
			APPL_INFO("==> STOP ... (START)");
			APPL_INFO("==> STOP ... (END)");
		}
		void onKillDemand(ewol::Context& _context) override {
			APPL_INFO("==> User demand kill ... (START)");
			_context.exit(0);
			APPL_INFO("==> User demand kill ... (END)");
		}
};

/**
 * @brief Main of the program (This can be set in every case, but it is not used in Andoid...).
 * @param std IO
 * @return std IO
 */
int main(int _argc, const char *_argv[]) {
	// second possibility
	return ewol::run(ETK_NEW(MainApplication), _argc, _argv);
}