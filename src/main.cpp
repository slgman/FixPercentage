#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(PlayLayer) {
    struct Fields {
        float startPosPercent = 0.f;
        float startPosTime    = 0.f;
        float totalLevelTime  = 0.f;
    };

    bool hasStartPos() {
        return !m_isPlatformer && m_startPosObject;
    }

    float timeAtX(float x) {
        return timeForPos({x, 0.f}, 0.f, 0.f, true, 0.f);
    }

    void resetLevel() {
        PlayLayer::resetLevel();

        auto& f = m_fields;
        f->startPosPercent = 0.f;
        f->startPosTime    = 0.f;
        f->totalLevelTime  = 0.f;

        if (!hasStartPos()) return;

        const float startX      = m_startPosObject->getPositionX();
        const float endTime     = timeAtX(m_levelLength);
        f->startPosTime         = timeAtX(startX);
        f->totalLevelTime       = endTime - f->startPosTime;
        f->startPosPercent      = (startX / m_levelLength) * 100.f;
    }

    float getCurrentPercent() {
        auto& f = m_fields;

        if (!hasStartPos() || f->startPosTime <= 0.f || f->totalLevelTime <= 0.f)
            return PlayLayer::getCurrentPercent();

        const float elapsed     = m_gameState.m_levelTime - f->startPosTime;
        const float progress    = elapsed / f->totalLevelTime;
        const float percent     = f->startPosPercent + progress * (100.f - f->startPosPercent);

        return std::clamp(percent, 0.f, 100.f);
    }

    void updateProgressbar() {
        if (!m_progressBar || !m_progressFill) return;

        const float pct = getCurrentPercent();

        const float fillWidth = m_progressFill->getContentSize().width;
        if (fillWidth > 0.f)
            m_progressFill->setScaleX((pct / 100.f) * m_progressWidth / fillWidth);

        if (!m_percentageLabel) return;

        const std::string label = formatPercent(pct);
        m_percentageLabel->setString(label.c_str());
    }

    std::string formatPercent(float pct) {
        const auto precision = Mod::get()->getSettingValue<std::string>("decimal-precision");

        if (precision == "vanilla")
            return fmt::format("{:.2f}%", pct);
        if (precision == "one")
            return fmt::format("{:.1f}%", pct);

        return fmt::format("{:.2f}%", pct);
    }
};
