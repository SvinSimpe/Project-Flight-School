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
#define TEAM_ARRAY_ID		mTeam - 1

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
	AssetID		playerModel[2];
	XMFLOAT3	direction;
};

struct LowerBody
{
	AnimationTrack	playerModel[2];
	XMFLOAT3		direction;
	XMFLOAT3		position;
	XMFLOAT4X4		rootMatrix;
};

struct Arms
{
	AnimationTrack leftArm;
	AnimationTrack rightArm;
};

struct Upgrades
{
	int		currentBodyLevel			= 1;
	float	damageTakenPercentage		= 1.0f; // 1 == 100 % == No resistance!
	int		currentLegsLevel			= 1;
	float	runSpeedFactor				= 0.7f;
};

class RemotePlayer
{
	// Member variables
	private:
	protected:
		UINT			mID;
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
		float			mLifeRegenerationAmount;	// Amount of HP regenerated when mLifeRegenerationTimer is 0.0
		float			mLifeRegenerationTimer;		// Time between regenerations
		int				mBufflevel;
		float			mSlowDown;
		Upgrades		mUpgrades;
		XMFLOAT3		mCurrentTravelVelocity;

		//Graphics
		Font			mFont;
		PointLight*		mPointLightIfDown;
		AssetID			mAnimations[PLAYER_ANIMATION::COUNT][2];
		AssetID			mWeaponModels[WEAPON_COUNT];
		AssetID			mWeaponAnimations[WEAPON_COUNT][WEAPON_ANIMATION_COUNT];
		UpperBody		mUpperBody;
		LowerBody		mLowerBody;
		Arms			mArms;
		bool			mLeftArmAnimationCompleted;
		bool			mRightArmAnimationCompleted;
		bool			mLegsAnimationCompleted;

		float			mPlayerDownSparksTimer;
		UINT			mEnergyCellID;

	public:

	// Member functions
	private:
		HRESULT			InitializeGraphics();
		void			EventListener( IEventPtr newEvent );

		float			GetHPBarScaleFactor() const;

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
		XMFLOAT3		GetVelocity() const;
		XMFLOAT3		GetCurrentVelocity() const;
		XMFLOAT3		GetWeaponOffset();
		std::string		GetName() const;
		UINT			GetEnergyCellID() const;
		void			SetDirection( XMFLOAT3 direction );
		void			SetHP( float hp );
		void			SetName( std::string name );
		void			SetEnergyCellID( UINT id );
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

