#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/Utils.hpp>

// this is really ugly

using namespace geode::prelude;

void forceColor(CCParticleSystem* system, const ccColor4F& col) {
    // virtuals are broken in geode
    system->CCParticleSystem::setStartColor(col);
    system->CCParticleSystem::setEndColor(col);
}

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

        auto col = ccc4FFromccc3B(newColor);
		forceColor(m_unk6dc, col);
		forceColor(m_trailingParticles, col);
		forceColor(m_shipClickParticles, col);
		forceColor(m_unk6e8, col);
		forceColor(m_ufoClickParticles, col);
		forceColor(m_robotBurstParticles, col);
		forceColor(m_unk6f4, col);
		forceColor(m_swingBurstParticles1, col);
		forceColor(m_swingBurstParticles2, col);
		forceColor(m_unk704, col);
		forceColor(m_unk708, col);
    }

    void updateGlowColor() {
        PlayerObject::updateGlowColor();
        this->updateGlowColorD(0.f);
    }
};