#pragma once

#include <SSVStart/SSVStart.hpp>

namespace Boilerplate
{
    class App
    {
    protected:
        using IK = ssvs::KKey;
        using IB = ssvs::MBtn;
        using IT = ssvs::Input::Type;
        using IM = ssvs::Input::Mode;

        ssvs::GameState gameState;
        ssvs::Camera gameCamera;
        ssvs::GameWindow& gameWindow;

    public:
        inline App(ssvs::GameWindow& mGameWindow)
            : gameCamera{mGameWindow, 1.f}, gameWindow{mGameWindow}
        {
        }

        inline void stop() noexcept { return gameWindow.stop(); }

        template <typename... TArgs>
        inline void render(TArgs&&... mArgs)
        {
            gameWindow.draw(FWD(mArgs)...);
        }

        inline auto& getGameState() noexcept { return gameState; }
        inline const auto& getGameState() const noexcept { return gameState; }
        inline auto& getGameCamera() noexcept { return gameCamera; }
        inline const auto& getGameCamera() const noexcept { return gameCamera; }
        inline auto& getGameWindow() noexcept { return gameWindow; }
        inline const auto& getGameWindow() const noexcept { return gameWindow; }
    };

    auto makeBitmapText()
    {
        ssvs::BitmapText result{ssvs::getDefaultAsset<ssvs::BitmapFont>()};
        result.setTracking(-3);
        return result;
    }

    class TestApp : public Boilerplate::App
    {
    protected:
        using FT = ssvs::FT;

    public:
        ssvu::Delegate<void(std::ostringstream&, FT)> onTxtInfoUpdate;

    private:
        FT txtInfoUpdateRate, txtInfoUpdateFT{0.f};
        std::ostringstream ossInfo;
        ssvs::BitmapText txtInfo{makeBitmapText()};

        void initTxtInfo()
        {
            txtInfo.setPosition(5.f, 5.f);
            txtInfo.setColor(sf::Color::White);
        }

        void initInput()
        {
            gameState.addInput({{IK::Escape}},
                [this](FT)
                {
                    stop();
                },
                IT::Once);
            gameState.addInput({{IK::A}}, [this](FT)
                {
                    gameCamera.pan(-4, 0);
                });
            gameState.addInput({{IK::D}}, [this](FT)
                {
                    gameCamera.pan(4, 0);
                });
            gameState.addInput({{IK::W}}, [this](FT)
                {
                    gameCamera.pan(0, -4);
                });
            gameState.addInput({{IK::S}}, [this](FT)
                {
                    gameCamera.pan(0, 4);
                });
            gameState.addInput({{IK::Q}}, [this](FT)
                {
                    gameCamera.zoomOut(1.1f);
                });
            gameState.addInput({{IK::E}}, [this](FT)
                {
                    gameCamera.zoomIn(1.1f);
                });
        }

        void updateTxtInfo(FT mFT)
        {
            txtInfoUpdateFT += mFT;
            if(txtInfoUpdateFT < txtInfoUpdateRate) return;

            txtInfoUpdateFT = 0;

            txtInfo.setString(ossInfo.str());
            ossInfo.str("");

            ossInfo << "FPS: " << gameWindow.getFPS() << "\n"
                    << "U: " << gameWindow.getMsUpdate() << "\t"
                    << "D: " << gameWindow.getMsDraw() << "\n";

            onTxtInfoUpdate(ossInfo, mFT);

            ossInfo << "\n";
        }

        void drawTxtInfo() { render(txtInfo); }

    public:
        TestApp(ssvs::GameWindow& mGameWindow) : Boilerplate::App{mGameWindow}
        {
            initTxtInfo();
            initInput();

            gameState.onUpdate += [this](FT mFT)
            {
                gameCamera.update(mFT);
                update(mFT);
            };

            gameState.onDraw += [this]
            {
                gameCamera.apply();
                draw();
                gameCamera.unapply();
            };

            enableTxtInfo(10.f);
        }

        virtual ~TestApp() {}

        void enableTxtInfo(FT mUpdateRate)
        {
            txtInfoUpdateRate = mUpdateRate;
            gameState.onUpdate += [this](FT mFT)
            {
                updateTxtInfo(mFT);
            };
            gameState.onDraw += [this]
            {
                drawTxtInfo();
            };
        }

        virtual void update(FT) {}
        virtual void draw() {}
    };
}