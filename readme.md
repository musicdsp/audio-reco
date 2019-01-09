audio-reco
==========

`audio-reco` is simple audio recognition engine build from scratch

Instructions
============

Note:

	depend on etk library (MPL v2.0)

To create a corpus:

	lutin.py audio-reco-wordRecorder?build?run

Analyse the corpus and create the "model":

	lutin.py audio-reco-analyser?build?run:--corpus=~/.local/shared/audio-reco-wordRecorder/corpus
	lutin.py audio-reco-analyser?build?run:--corpus=~/.local/shared/audio-reco-wordRecorder/corpus:--help
	lutin -cclang -mrelease audio-reco-wordRecorder?build?run:--elog-level=3:--user_name=Edouard_DUPIN:--user_birth_year=1983:--source_type=ORDERED_WORD
	

License (MPL v2.0)
=====================
Copyright audio Edouard DUPIN

Licensed under the Mozilla Public License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://www.mozilla.org/MPL/2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
