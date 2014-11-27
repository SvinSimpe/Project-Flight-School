#ifndef INPUT_H
#define INPUT_H

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#include <vector>

const UINT NROFDEVICES	= 2;	//The amount of devices to be registered; mouse and keyboard currently, added to the array in written order.
const UINT BUFFERSIZE	= 40;	//Fixed buffer size for interpretating RAWINPUT messages, set to the size of the mouse input since it is the biggest of the 2 devices we're handling.

class Input
{
	private:
	protected:
	public:
		RAWINPUTDEVICE mRid[NROFDEVICES];
		LPBYTE lpb[BUFFERSIZE];

	private:	
		//Stoping the compiler from generating methods for copying the object
				Input( Input const& );		//Not implemented
		void	operator=( Input const& );	//Not implemented
				//Private constructor
				Input() {};

	protected:
	public:
		RAWINPUT*	ReadMessage( LPARAM lParam );
		HRESULT		Initialize();
		void		Release();

		static Input* GetInstance()
		{
			static Input instance;
			return &instance;
		}
		~Input(){};
};

#endif