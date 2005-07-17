
#include <string>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

#include "MimeType.h"

bool MimeType::isInitialized = false;
map<string, string> MimeType::mimeMap;
map<string, string> MimeType::mimeCache;

bool MimeType::_matches(string pattern, string ext) {

	if (pattern[0] == '*') {
		pattern = pattern.substr(1);
		unsigned int pos = ext.rfind(pattern);
		if (pos == string::npos)
			return false;
		return ext.length() - pos == pattern.length();
	}

	if (pattern[pattern.length() - 1] == '*') {
		pattern = pattern.substr(0, pattern.length() - 1);
		return ext.find(pattern) == 0;
	}

	return ext == pattern;

}

string MimeType::getMimeFromName(string name) {

	if (!isInitialized)
		__initialize();

	if (mimeCache[name] != "")
		return mimeCache[name];

	string ext;

	if (name.find(".") == string::npos) {

		ext = name;

	} else {

		ext = name.substr(name.rfind("."));

		if (name.find(".gz") != string::npos)
			ext = name.substr(name.rfind(".", 4));
		if (name.find(".bz2") != string::npos)
			ext = name.substr(name.rfind(".", 5));

	}

	string rv = "application/octet-stream";

	for (map<string, string>::iterator it = mimeMap.begin(); it != mimeMap.end(); ++it) {

		if (_matches(it->first, ext)) {
			rv = it->second;
			break;
		}

	}

	mimeCache[name] = rv;

	return rv;

}

void MimeType::__initialize(void) {

	if (isInitialized)
		return;

#ifndef WIN32
	ifstream ifs("/usr/share/mime/globs", ios::in);

	if (!ifs) {
		cerr << "ifs() failed" << endl;
		return;
	}

	mimeMap.clear();
	mimeCache.clear();

	while (!ifs.eof()) {

		string tmp;
		getline(ifs, tmp);

		if (tmp == "" || tmp[0] == '#')
			continue;

		string mime, pattern;
		mime = tmp.substr(0, tmp.find(":"));
		pattern = tmp.substr(tmp.find(":") + 1);

		mimeMap[pattern] = mime;

	}

	ifs.close();
#else
	_NOT_IMPLEMENTED_;
#endif

	// Some missing globs:
	mimeMap["*.asc"] = "application/pgp-encrypted";
	mimeMap["*.com"] = "application/x-ms-dos-executable";
	mimeMap["*.conf"] = "text/x-config";
	mimeMap["*.dll"] = "application/x-ms-dos-executable";
	mimeMap["*.ebuild"] = "text/ebuild";
	mimeMap["*.eclass"] = "text/eclass";
	mimeMap["*.gpg"] = "application/pgp-keys";
	mimeMap["*.h++"] = "text/x-c++hdr";
	mimeMap["*.hpp"] = "text/x-c++hdr";
	mimeMap["*.ini"] = "text/x-config";
	mimeMap["*.jsp"] = "text/x-jsp";
	mimeMap["*.key"] = "application/pgp-keys";
	mimeMap["*.l"] = "text/x-lex";
	mimeMap["*.latex"] = "text/x-tex";
	mimeMap["*.mak"] = "text/x-makefile";
	mimeMap["*.ogm"] = "video/vorbis";
	mimeMap["*.tbz2"] = "application/x-bzip-compresses-tar";
	mimeMap["*.wma"] = "application/x-ms-wma";
	mimeMap["*.y"] = "text/x-yacc";
	mimeMap["*rc"] = "text/x-config";

	isInitialized = true;

}
