#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H

#include "Input.h"
#include "Graphics.h"
#include "Events.h"
#include "BoundingGeometry.h"
#include "RenderManager.h"
#include "Font.h"
#include "WeaponInfo.h"

#define MAX_NR_OF_PLAYERS	8

#define LEFT_ARM_ID			0
#define RIGHT_ARM_ID		1

enum PLAYER_ANIMATION
{
	LEGS_IDLE,
	LEGS_WALK,
	LEGS_DEATH,
	LEGS_DOWN,
	COUNT,
};

enum WEAPON_ANIMATION
{
	IDLE,
	WALK,
	ATTACK,
	ATTACK2,
	ATTACK_START,
	ATTACK_IDLE,
	ATTACK_END,
	OVERHEAT_START,
	OVERHEAT_END,

	WEAPON_ANIMATION_COUNT,
};

struct UpperBody
{
	AssetID		playerModel;
	XMFLOAT3	direction;
};

struct LowerBody
{
	AnimationTrack	playerModel;
	XMFLOAT3		direction;
	XMFLOAT3		position;
	XMFLOAT4X4		rootMatrix;
};

struct Arms
{
	AnimationTrack leftArm;
	AnimationTrack rightArm;
};

class RemotePlayer
{
	// Member variables
	private:
	protected:
		unsigned int	mID;
		std::string		mPlayerName;
		int				mTeam;
		bool			mIsAlive;
		bool			mIsDown;
		float			mCurrentHp;
		float			mMaxHp;
		int				mNrOfDeaths;
		int				mNrOfKills;
		float			mDeathTime;
		float			mTimeTillDeath;
		float			mSpawnTime;
		float			mTimeTillSpawn;
		XMFLOAT3		mVelocity;
		LoadOut*		mLoadOut;
		BoundingRectangle*	mBoundingBox;
		BoundingCircle*	mBoundingCircle;
		BoundingCircle*	mBoundingCircleAura;
		XMFLOAT3		mSpawnPosition;
		bool			mIsBuffed;
		float			mBuffMod; // Modifies the damage a player takes by a percentage, should only range between 0 and 1

		//Graphics
		Font			mFont;
		PointLight*		mPointLightIfDown;
		AssetID			mAnimations[PLAYER_ANIMATION::COUNT];
		AssetID			mWeaponModels[WEAPON_COUNT];
		AssetID			mWeaponAnimations[WEAPON_COUNT][WEAPON_ANIMATION_COUNT];
		UpperBody		mUpperBody;
		LowerBody		mLowerBody;
		Arms			mArms;
		bool			mLeftArmAnimationCompleted;
		bool			mRightArmAnimationCompleted;
		bool			mLegsAnimationCompleted;

		float			mPlayerDownSparksTimer;

	public:

	// Member functions
	private:
		HRESULT			InitializeGraphics();
		void			EventListener( IEventPtr newEvent );
	protected:

	public:
		virtual void	Die();
		void			Spawn();
		void			CountUpKills();
		virtual void	GoDown();
		virtual void	GoUp();
		virtual HRESULT	Update( float deltaTime );
		virtual HRESULT	Render();
		virtual HRESULT	Initialize();
		void			RemoteInit( unsigned int id, int team );
		void			Release();
						RemotePlayer();
		virtual			~RemotePlayer();

		//GetSet
		bool			IsAlive() const;
		bool			IsDown() const;
		bool			IsBuffed() const;
		LoadOut*		GetLoadOut() const;
		float			GetHP() const;
		float			GetMaxHP() const;
		int				GetID() const;
		int				GetTeam() const;
		BoundingRectangle*	GetBoundingBox() const;
		BoundingCircle*	GetBoundingCircle() const;
		BoundingCircle*	GetBoundingCircleAura() const;
		XMFLOAT3		GetPosition() const;
		XMFLOAT3		GetDirection() const;
		std::string		GetName() const;
		void			SetDirection( XMFLOAT3 direction );
		void			SetHP( float hp );
		void			SetName( std::string name );
};

// Server player
struct ServerPlayer
{
		UINT					ID;
		UINT					TeamID;
		XMFLOAT3				Pos			= XMFLOAT3( 0.0f, 0.0f, 0.0f );
		bool					IsBuffed	= false;
		bool					IsAlive		= false;
		bool					IsDown		= false;
		BoundingCircle*			AggroCircle;

		~ServerPlayer()
		{
			SAFE_DELETE( AggroCircle );
		}
};

#endif

