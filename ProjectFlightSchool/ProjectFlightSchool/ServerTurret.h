#ifndef SERVERTURRET_H
#define SERVERTURRET_H

class ServerTurret
{
	private:
		enum class TurretMode
		{
			Idle,
			Attack,
		};

		class ITurretMode
		{
		public:
			virtual void Action( ServerTurret* t ) = 0;
					ITurretMode() {}
			virtual ~ITurretMode() {}
		};

		class IdleTurret : public ITurretMode
		{
		public:
			void Action( ServerTurret* t );
			IdleTurret() : ITurretMode() {}
			~IdleTurret() {}
		};

		class AttackingTurret : public ITurretMode
		{
		public:
			void Action( ServerTurret* t );
			AttackingTurret() : ITurretMode() {}
			~AttackingTurret() {}
		};

	protected:
	public:

	private:
	protected:
	public:
		ServerTurret();
		virtual ~ServerTurret();
};
#endif