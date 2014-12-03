#include "MapPathImportHandler.h"

///////////////////////////////////////////////////////////////////////////////
//									PRIVATE
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//									PUBLIC
///////////////////////////////////////////////////////////////////////////////
HRESULT MapPathImportHandler::HandlePaManPath( string folderPath )		//Goes though the files in a given folder and imports the .PaMan animation files
{
	string	folderPathFormat = folderPath + "*.PaMan*";
	wstring stemp = wstring( folderPathFormat.begin(), folderPathFormat.end() );
	LPCWSTR pathAsLpcwstr = stemp.c_str();

	HANDLE hFind;
	WIN32_FIND_DATA	data;
	ImporterAnim animImport;
	
	hFind = FindFirstFile( pathAsLpcwstr, &data );
	if ( hFind != INVALID_HANDLE_VALUE ) {
		do {
			wstring fileName( data.cFileName );
			string	tempPath( fileName.begin(), fileName.end() );

			cout << "Reading File: " << folderPath + tempPath << "\n";
			mAnimationList.push_back( animImport.ImportBinaryAnimData( folderPath, tempPath ) );
			
		} while ( FindNextFile( hFind, &data ) );
		FindClose( hFind );
	}
	return S_OK;
}

HRESULT MapPathImportHandler::HandleSkelPath(string folderPath)		//Goes though the files in a given folder and imports the .Skel files
{
	string	folderPathFormat = folderPath + "*.Skel*";
	wstring stemp = wstring(folderPathFormat.begin(), folderPathFormat.end());
	LPCWSTR pathAsLpcwstr = stemp.c_str();

	HANDLE hFind;
	WIN32_FIND_DATA	data;
	ImporterAnim animImport;

	hFind = FindFirstFile(pathAsLpcwstr, &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			wstring fileName(data.cFileName);
			string	tempPath(fileName.begin(), fileName.end());

			cout << "Reading File: " << folderPath + tempPath << "\n";
			mSkeletonList.push_back( animImport.ImportBinarySkelData( folderPath, tempPath ) );

		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
	return S_OK;
}

MapPathImportHandler::MapPathImportHandler()
{

}

MapPathImportHandler::~MapPathImportHandler()
{

}