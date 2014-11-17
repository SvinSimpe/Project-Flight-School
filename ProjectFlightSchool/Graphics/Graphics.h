#ifndef GRAPHICS_H
#define GRAPHICS_H

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dcompiler.lib" )

#include <d3d11.h>
#include <d3dcompiler.h>

#define SAFE_RELEASE( x ) if( x ) { ( x )->Release(); ( x ) = nullptr; }

#ifdef COMPILE_LIBRARY
	#define LIBRARY_EXPORT __declspec( dllexport )
#else
	#define LIBRARY_EXPORT __declspec( dllimport )
#endif

class LIBRARY_EXPORT Graphics
{
	//Class members
	private:

	protected:
	public:

	//Class functions
	private:
		Graphics();
		virtual	~Graphics();

	protected:
	public:
		HRESULT Initialize();
		void	Release();
		static	Graphics* GetInstance();
		
};
#endif