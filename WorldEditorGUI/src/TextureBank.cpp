//=============================================================================
#include "TextureBank.h"
#include "App.h"
#include <filesystem>
#include "FileManager.h"
//#include "Log.h"

//=============================================================================
std::map<std::string, Texture*> TextureBank::TexList;

//=============================================================================
bool TextureBank::Init() {
	Cleanup();

	SDL_Renderer* Renderer = App::GetInstance()->GetRenderer();
	if (!Renderer) return false;

	//std::vector<std::string> Files = FileManager::GetFilesInFolder("Textures"); // Relative to CWD

	namespace fs = std::filesystem;
	const std::string textures_dir = "res/textures";

	//for (const auto& outer_dir : fs::directory_iterator(shadows_folder_path))
	{
		for (const auto& entry : fs::directory_iterator(textures_dir))
		{
			//std::cout << "test = " << entry.path().filename() << std::endl;
			const std::string Ext = entry.path().filename().extension().string();
			const std::string ID = entry.path().stem().string();

			/*std::string Ext = FileManager::GetFilenameExt(entry.path().string());
			std::string ID = FileManager::GetFilenameWithoutExt(entry.path().string());*/

			if (Ext == ".png")
			{
				AddTexture(Renderer, ID, entry.path().string());
			}
			//std::cout << entry.path() << std::endl;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
void TextureBank::Cleanup() {
	if (TexList.size() <= 0) return;

	for (auto& Iterator : TexList) {
		Texture* TheTexture = (Texture*)Iterator.second;

		if (TheTexture) {
			delete TheTexture;
			TheTexture = NULL;
		}
	}

	TexList.clear();
}

//=============================================================================
void TextureBank::AddTexture(SDL_Renderer* Renderer, std::string ID, std::string Filename) {
	if (ID == "") return;

	Texture* NewTexture = new Texture();
	if (NewTexture->Load(Renderer, Filename) == false) {
		//Log("Unable to Load Texture: %s", ID.c_str());
		return;
	}

	TexList[ID] = NewTexture;
}

//-----------------------------------------------------------------------------
Texture* TextureBank::Get(std::string ID) {
	if (TexList.find(ID) == TexList.end()) return 0;

	return TexList[ID];
}

//=============================================================================