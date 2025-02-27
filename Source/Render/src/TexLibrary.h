#pragma once

class cTexture;
class cTextureScale;

class cTexLibrary
{
public:
	cTexLibrary();
	~cTexLibrary();
	
	void Free(FILE* f=NULL);
	void Compact(FILE* f=NULL);

	bool EnableError(bool enable);
	cTexture* GetElementAviScale(const char* TextureName,char *pMode =0);
	cTexture* GetElement(const char *pTextureName,char *pMode=0);
	cTextureScale* GetElementScale(const char *pTextureName,Vect2f scale);
	cTexture* GetElementColor(const char *pTextureName,sColor4c color,char *pMode=0);

	inline int GetNumberTexture()							{ return textures.size(); }
	inline cTexture* GetTexture(int number)					{ return textures[number]; }

	cTexture* CreateRenderTexture(int width,int height,DWORD attr=0,bool enable_assert=true);
	cTexture* CreateTexture(int sizex,int sizey,bool alpha);
	cTexture* CreateTextureDefaultPool(int sizex,int sizey,bool alpha);

	MTSection* GetLock(){return &lock;}
	void ReloadAllTexture();
private:
	cTexture* CreateTexture(int sizex,int sizey,bool alpha,bool default_pool);
	bool enable_error;
	std::vector<cTexture*> textures;
	void FreeOne(FILE* f);

	bool LoadTexture(cTexture* Texture,char *pMode,Vect2f kscale);
	bool ReLoadTexture(cTexture* Texture,Vect2f kscale);
	bool ReLoadTexture(cTexture* Texture);

	void Error(cTexture* Texture);

	bool ReLoadDDS(cTexture* Texture);
	MTSection lock;
};

cTexLibrary* GetTexLibrary();
