#include "StdAfx.h"
#include "BinaryArchive.h"

///////////////////////////////////////////////////////////////////////////////////////
//			ScriptParser
///////////////////////////////////////////////////////////////////////////////////////
BinaryOArchive::BinaryOArchive(const char* fname, int version) :
buffer_(10, 1)
{
	open(fname, version);
}

BinaryOArchive::~BinaryOArchive() 
{
	close();
}

void BinaryOArchive::open(const char* fname, int version)
{
	fileName_ = fname;
	buffer_.init();
	buffer_.alloc(10000);
	buffer_ < "BinX" < version;
}

bool BinaryOArchive::close()
{
	XStream ff(0);
	if(ff.open(fileName_.c_str(), XS_IN)){
		if(ff.size() == buffer_.tell()){
			PtrHandle<char> buf = new char[ff.size()];
			ff.read(buf, ff.size());
			if(!memcmp(buffer_, buf, buffer_.tell()))
				return true;
		}
	}
	ff.close();
	ff.open(fileName_.c_str(), XS_OUT);
	ff.write(buffer_.address(), buffer_.tell());
	return !ff.ioError();
}

//////////////////////////////////////////////
BinaryIArchive::BinaryIArchive(const char* fname) :
buffer_(10, 1)
{
	if(fname && !open(fname))
		ErrH.Abort("File not found: ", XERR_USER, 0, fname);
}

BinaryIArchive::~BinaryIArchive() 
{
	close();
}

bool BinaryIArchive::open(const char* fname)
{
	fileName_ = fname;
	XStream ff(0);
	if(!ff.open(fname, XS_IN))
		return false;
	buffer_.alloc(ff.size() + 1);
	ff.read(buffer_.address(), ff.size());
	buffer_[(int)ff.size()] = 0;
	if(buffer_[0] != 'B' || buffer_[1] != 'i' || buffer_[2] != 'n' || buffer_[3] != 'X'){
		close();
		return false;
	}
	(buffer_ += 4) > version_;
	return true;
}

void BinaryIArchive::close()
{
	buffer_.alloc(10);
}

bool BinaryIArchive::loadString(std::string& str)
{
	char c;
	buffer_ > c;
	if(c){
		str = &buffer_();
		while(buffer_())
			++buffer_;
		++buffer_;
		return true;
	}
	else{
		str = "";
		return false;
	}
}

