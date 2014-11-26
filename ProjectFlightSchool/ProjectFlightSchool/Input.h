#ifndef INPUT_H
#define INPUT_H

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#include <vector>

#define NROFDEVICES 2	//The amount of devices to be registered; currently handling mouse and keyboard.
class Input
{
	private:
	protected:
	public:
		RAWINPUTDEVICE mRid[2];

	private:	
		//Stoping the compiler from generating methods for copying the object
				Input(Input const&);		//Not implemented
		void	operator=(Input const&);	//Not implemented
				//Private constructor
				Input() {};

	protected:
	public:
		RAWINPUT*	ReadMessage(LPARAM lParam);
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