#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(PlayLayer) {
    bool hasStartPos() {
        return !m_isPlatformer && m_startPosObject;
    }

    float getCurrentPercent() {
        if (!hasStartPos()) return PlayLayer::getCurrentPercent();

        float pct = 0.f;
        if (m_level->m_timestamp > 0) pct = (m_gameState.m_levelTime * 240.f) / m_level->m_timestamp * 100.f;
        else if (m_levelLength > 0.f) pct = (m_player1->getPositionX() / m_levelLength) * 100.f;

        return std::clamp(pct, 0.f, 100.f);
    }

    void updateProgressbar() {
        if (!m_progressBar || !m_progressFill) return;

        float pct = getCurrentPercent() / 100.f;
        float newWidth = m_progressWidth * pct;
        auto currentSize = m_progressFill->getContentSize();

        m_progressFill->setTextureRect({0, 0, newWidth, currentSize.height});

        if (!m_percentageLabel) return;
        m_percentageLabel->setString(formatPercent(getCurrentPercent()).c_str());
    }

    std::string formatPercent(float pct) {
        const auto precision = Mod::get()->getSettingValue<std::string>("decimal-precision");

        if (precision == "vanilla") return fmt::format("{:.0f}%", pct);
        if (precision == "one") return fmt::format("{:.1f}%", pct);
        return fmt::format("{:.2f}%", pct);
    }
};
