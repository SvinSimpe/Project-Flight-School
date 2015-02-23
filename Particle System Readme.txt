
------------------------------------------------
--------------- QUESTIONS??? ---------------
------------------------------------------------
Who you gonna call? SvinSimpe!!
================================================


1. Add ParticleType in ParticleData aswell as increment NR_OF_PARTICLE_TYPES

2. Add initialize for your new particle type in ParticleManager::Initialize

3. In ParticleSystem::Initialize, add case for your new particle type regarding asset

4. In ParticleSystem::Emitter, add statement

5. In ParticleSystem::Generate, add statement

6. In ParticleData::SetDirection, add statement

	6.1. If needed, add statement in ParticleData::GetRandomSpread

7. Add function for new particle logic

8. In ParticleSystem::Update, add case for new particle logic

9. Create effect! (Change to text from HLSL Compiler)

10. Particle Effects in Effect enum in Graphics.h must correspond to ParticleType in ParticleData

11. Add effect initialize in Graphics::InitializeEffect

12. Request effect with:   RenderManager::GetInstance()->RequestParticleSystem

13. Add Particle type in Graphics::RenderParticleSystem if you want Additive Blending


------------------------------------------------
--------------- QUESTIONS??? ---------------
------------------------------------------------
Who you gonna call? SvinSimpe!!
================================================
