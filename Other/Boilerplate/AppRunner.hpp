#pragma once

#include "./App.hpp"
#include <SSVStart/SSVStart.hpp>

namespace Boilerplate
{
    template <typename T>
    class AppRunner
    {
    private:
        ssvs::GameWindow gameWindow;
        ssvu::UPtr<T> app;

    public:
        inline AppRunner(
            const std::string& mTitle, ssvu::SizeT mWidth, ssvu::SizeT mHeight)
        {
            gameWindow.setTitle(mTitle);
            gameWindow.setTimer<ssvs::TimerStatic>(0.5f, 0.5f);
            gameWindow.setSize(mWidth, mHeight);
            gameWindow.setFullscreen(false);
            gameWindow.setFPSLimited(true);
            gameWindow.setMaxFPS(200);
            gameWindow.setPixelMult(2);

            app = ssvu::mkUPtr<T>(gameWindow);

            gameWindow.setGameState(app->getGameState());
            gameWindow.run();
        }
    };
}