#ifndef HEAD_H
#define HEAD_H

// Used by both server and client in order for them to know what to do with a given package
enum class Operation
{
	NONE,
	MESSAGE
};

// Contains information about the package such as where to send it, what to do with it, and the size of the content to be sent
class Head
{
	private:
		int			mSocketID;
		Operation	mOperation;
		int			mContentSize;

	protected:
	public:

	private:
	protected:
	public:
		bool	Initialize(int socketID, Operation operation, int size);
		void	Release();
				Head();
		virtual ~Head();
};
#endif