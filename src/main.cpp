#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/CCParticleSystem.hpp>
#include <Geode/Utils.hpp>

// this is really ugly

using namespace geode::prelude;

class $modify(MyParticleSystem, CCParticleSystem) {
	auto getPParticles() {
		return m_pParticles;
	}

    void forceColor(const ccColor4F& col) {
        this->CCParticleSystem::setStartColor(col);
        this->CCParticleSystem::setEndColor(col);

        // auto pp = this->getPParticles();
        // size_t count = this->CCParticleSystem::getParticleCount();

        // for (size_t i = 0; i < count; i++) {
        //     tCCParticle& p = pp[i];
        //     p.color = col;
        // }
    }
};

static inline ccColor3B lerpColor(ccColor3B from, ccColor3B to, float delta) {
    delta = std::clamp(delta, 0.f, 1.f);

    ccColor3B out;
    out.r = std::lerp(from.r, to.r, delta);
    out.g = std::lerp(from.g, to.g, delta);
    out.b = std::lerp(from.b, to.b, delta);

    return out;
}

ccColor3B COLORS[] = {
    cc3bFromHexString("#cd66ff").value(),
    cc3bFromHexString("#ff6599").value(),
    cc3bFromHexString("#ff0000").value(),
    cc3bFromHexString("#ff8e00").value(),
    cc3bFromHexString("#ffff00").value(),
    cc3bFromHexString("#008e00").value(),
    cc3bFromHexString("#00c0c0").value(),
    cc3bFromHexString("#400098").value(),
    cc3bFromHexString("#be00be").value(),

    cc3bFromHexString("#cd66ff").value(),
};

class $modify(HookedPlayerObject, PlayerObject) {
    float tpColorDelta = 0.f;
    size_t curIdx = 0;

	bool init(int p0, int p1, GJBaseGameLayer* p2, cocos2d::CCLayer* p3, bool p4) {
		if (!PlayerObject::init(p0, p1, p2, p3, p4)) return false;

        // log::debug("m_unk6dc: {}", m_unk6dc);
        // log::debug("m_trailingParticles: {}", m_trailingParticles);
        // log::debug("m_shipClickParticles: {}", m_shipClickParticles);
        // log::debug("m_unk6e8: {}", m_unk6e8);
        // log::debug("m_ufoClickParticles: {}", m_ufoClickParticles);
        // log::debug("m_robotBurstParticles: {}", m_robotBurstParticles);
        // log::debug("m_unk6f4: {}", m_unk6f4);
        // log::debug("m_swingBurstParticles1: {}", m_swingBurstParticles1);
        // log::debug("m_swingBurstParticles2: {}", m_swingBurstParticles2);
        // log::debug("m_unk704: {}", m_unk704);
        // log::debug("m_unk708: {}", m_unk708);

        this->schedule(schedule_selector(HookedPlayerObject::updateGlowColorD));

		return true;
	}

    void updateGlowColorD(float dt) {
        constexpr float interval = 0.2f;

        m_fields->tpColorDelta += dt;

        auto newColor = lerpColor(COLORS[m_fields->curIdx], COLORS[m_fields->curIdx + 1], m_fields->tpColorDelta / interval);

        if (m_fields->tpColorDelta >= interval) {
            m_fields->curIdx++;
            m_fields->tpColorDelta = std::fmod(m_fields->tpColorDelta, interval);
            if (m_fields->curIdx == 7) {
                m_fields->curIdx = 0;
            }
        }

        // log::debug("forcing to {}", newColor);

        auto col = ccc4FFromccc3B(newColor);
		static_cast<MyParticleSystem*>(static_cast<CCParticleSystem*>(m_unk6dc))->forceColor(col);
		static_cast<MyParticleSystem*>(static_cast<CCParticleSystem*>(m_trailingParticles))->forceColor(col);
		static_cast<MyParticleSystem*>(static_cast<CCParticleSystem*>(m_shipClickParticles))->forceColor(col);
		static_cast<MyParticleSystem*>(static_cast<CCParticleSystem*>(m_unk6e8))->forceColor(col);
		static_cast<MyParticleSystem*>(static_cast<CCParticleSystem*>(m_ufoClickParticles))->forceColor(col);
		static_cast<MyParticleSystem*>(static_cast<CCParticleSystem*>(m_robotBurstParticles))->forceColor(col);
		static_cast<MyParticleSystem*>(static_cast<CCParticleSystem*>(m_unk6f4))->forceColor(col);
		static_cast<MyParticleSystem*>(static_cast<CCParticleSystem*>(m_swingBurstParticles1))->forceColor(col);
		static_cast<MyParticleSystem*>(static_cast<CCParticleSystem*>(m_swingBurstParticles2))->forceColor(col);
		static_cast<MyParticleSystem*>(static_cast<CCParticleSystem*>(m_unk704))->forceColor(col);
		static_cast<MyParticleSystem*>(static_cast<CCParticleSystem*>(m_unk708))->forceColor(col);
    }

    void updateGlowColor() {
        PlayerObject::updateGlowColor();
        this->updateGlowColorD(0.f);
    }
};