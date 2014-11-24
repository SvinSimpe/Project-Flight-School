#ifndef BODY_H
#define BODY_H

// Part of the package-class, used to store content of a message
class Body
{
	private:
		char* mContent;

	protected:
	public:

	private:
	protected:
	public:
		bool	Initialize(char* content);
		void	Release();
				Body();
		virtual ~Body();
};
#endif