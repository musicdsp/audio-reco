/** @file
 * @author Edouard DUPIN 
 * @copyright 2019, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */


#include <etk/types.hpp>
#include <etk/etk.hpp>
#include <etk/uri/uri.hpp>

#include <appl/debug.hpp>
#include <etk/theme/theme.hpp>
#include <ejson/ejson.hpp>
#include <audio/algo/speex/Resampler.hpp>

static int resampling_quality = 10; // speex resampler quality

/**
 * @brief Main of the program (This can be set in every case, but it is not used in Andoid...).
 * @param std IO
 * @return std IO
 */
int main(int _argc, const char *_argv[]) {
	etk::init(_argc, _argv);
	
	
	etk::Vector<etk::Uri> list = etk::uri::listRecursive("FILE:///framework/musicdsp/audio-reco-corpus/corpus1/");
	auto it = list.begin();
	while (it != list.end()) {
		if (it->getPath().getExtention() == "json") {
			++it;
		} else {
			it = list.erase(it);
		}
	}
	for (auto &it: list) {
		APPL_DEBUG("elem=" << it);
		ejson::Document doc;
		doc.load(it);
		etk::String fileName = doc["audio_filename"].toString().get();
		etk::Uri fileNameAudioFile = it;
		fileNameAudioFile.setPath(fileNameAudioFile.getPath().getParent() / fileName);
		etk::Uri fileNameAudioFile16k = it;
		fileNameAudioFile16k.setPath(fileNameAudioFile16k.getPath().getParent() / fileName + "_16k");
		APPL_DEBUG("    file=" << fileName);
		int32_t sampleRate = doc["audio_sample_rate"].toNumber().get();
		APPL_DEBUG("    sampleRate=" << sampleRate);
		etk::Vector<int16_t> dataBuffer;
		if (sampleRate != 16000) {
			if (etk::uri::exist(fileNameAudioFile16k) == false) {
				APPL_DEBUG("        *** LOAD_FILE ***");
				// read stream:
				etk::Vector<int16_t> bufferInput;
				{
					ememory::SharedPtr<etk::io::Interface> fileIO = etk::uri::get(fileNameAudioFile);
					if (fileIO->open(etk::io::OpenMode::Read) == false) {
						return -1;
					}
					bufferInput = fileIO->readAll<int16_t>();
					fileIO->close();
				}
				APPL_DEBUG("        *** RESAMPLE ***");
				dataBuffer.resize(bufferInput.size(), 0.0f);
				// Resample stream:
				audio::algo::speex::Resampler algo;
				algo.init(1, sampleRate, 16000, resampling_quality, audio::format_int16);
				size_t sizeOut = dataBuffer.size();
				/*
				for (int32_t iii=0; iii<1024; ++iii) {
					size_t sizeOut = output.size();
					algo.process(&output[0], sizeOut, &input[0], input.size());
				}
				*/
				algo.process(&dataBuffer[0], sizeOut, &bufferInput[0], bufferInput.size());
				dataBuffer.resize(sizeOut);
				APPL_DEBUG("        *** Store ***");
				{
					ememory::SharedPtr<etk::io::Interface> fileIO = etk::uri::get(fileNameAudioFile16k);
					if (fileIO->open(etk::io::OpenMode::Write) == false) {
						return -1;
					}
					fileIO->writeAll(dataBuffer);
					fileIO->close();
				}
			} else {
				APPL_DEBUG("        *** LOAD_FILE ***");
				// read stream: (We already have resample)
				{
					ememory::SharedPtr<etk::io::Interface> fileIO = etk::uri::get(fileNameAudioFile16k);
					if (fileIO->open(etk::io::OpenMode::Read) == false) {
						return -1;
					}
					dataBuffer = fileIO->readAll<int16_t>();
					fileIO->close();
				}
			}
		} else {
			// the input file is already in 16k
			ememory::SharedPtr<etk::io::Interface> fileIO = etk::uri::get(fileNameAudioFile);
			if (fileIO->open(etk::io::OpenMode::Read) == false) {
				return -1;
			}
			dataBuffer = fileIO->readAll<int16_t>();
			fileIO->close();
		}
	}
	
	return 0;
}