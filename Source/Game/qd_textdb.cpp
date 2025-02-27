/* ---------------------------- INCLUDE SECTION ----------------------------- */

#include "StdAfx.h"

#include "xutil.h"
#include "tweaks.h"
#include "qd_textdb.h"

/* ----------------------------- STRUCT SECTION ----------------------------- */
/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */
/* --------------------------- DEFINITION SECTION --------------------------- */

qdTextDB::qdTextDB()
{
}

qdTextDB::~qdTextDB()
{
	clear();
}

qdTextDB& qdTextDB::instance()
{
	static qdTextDB db;
	return db;
}

const char* qdTextDB::getText(const char* text_id) const
{
	qdTextMap::const_iterator it = texts_.find(text_id);
	if(it != texts_.end())
		return it->second.text_.c_str();

	static const char* const str = "";
	return str;
}

const char* qdTextDB::getSound(const char* text_id) const
{
	qdTextMap::const_iterator it = texts_.find(text_id);
	if(it != texts_.end())
		return it->second.sound_.c_str();

	static const char* const str = "";
	return str;
}

const char* qdTextDB::getComment(const char* text_id) const
{
#ifndef _FINAL_VERSION_
	qdTextMap::const_iterator it = texts_.find(text_id);
	if(it != texts_.end())
		return it->second.comment_.c_str();
#endif

	static const char* const str = "";
	return str;
}

bool qdTextDB::load(const char* file_name,const char* comments_file_name,bool clear_old_texts)
{
	XStream fh(0);
	if(!fh.open(convert_path_resource(file_name),XS_IN))
		return false;

	if(clear_old_texts) clear();

	int text_cnt;
	fh > text_cnt;

	std::string id_str(1024,0);
	std::string txt_str(1024,0);
	std::string snd_str(1024,0);
	for(int i = 0; i < text_cnt; i++){
		int id_length;
		fh > id_length;

		id_str.resize(id_length);
		fh.read(&*id_str.begin(),id_length);

		int txt_length;
		fh > txt_length;

		txt_str.resize(txt_length);
		fh.read(&*txt_str.begin(),txt_length);

		int snd_length;
		fh > snd_length;

		snd_str.resize(snd_length);
		fh.read(&*snd_str.begin(),snd_length);

		texts_.insert(qdTextMap::value_type(id_str.c_str(),qdText(txt_str.c_str(),snd_str.c_str())));
	}

	fh.close();

#ifndef _FINAL_VERSION_
	if(comments_file_name){
		if(!fh.open(convert_path_resource(comments_file_name),XS_IN))
			return true;

		fh > text_cnt;
		for(int i = 0; i < text_cnt; i++){
			int id_length;
			fh > id_length;

			id_str.resize(id_length);
			fh.read(&*id_str.begin(),id_length);

			int txt_length;
			fh > txt_length;

			txt_str.resize(txt_length);
			fh.read(&*txt_str.begin(),txt_length);

			int snd_length;
			fh > snd_length;

			snd_str.resize(snd_length);
			fh.read(&*snd_str.begin(),snd_length);

			qdTextMap::iterator it = texts_.find(id_str.c_str());
			if(it != texts_.end())
				it->second.comment_ = txt_str.c_str();
		}
	}
#endif

	return true;
}

void qdTextDB::getIdList(const char* mask, IdList& idList)
{
	idList.clear();
	qdTextMap::iterator i;
	FOR_EACH(texts_, i){
		std::string str = i->first;
		if(!i->first.find(mask)){
			std::string str = i->first;
			str.erase(0, strlen(mask) + 1);
			size_t pos = str.find(".");
			if(pos != std::string::npos)
				str.erase(pos, str.size());
			if(std::find(idList.begin(), idList.end(), str) == idList.end())
				idList.push_back(str);
		}
	}

	idList.sort();
}
