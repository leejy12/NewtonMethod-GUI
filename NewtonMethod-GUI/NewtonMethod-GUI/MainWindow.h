#pragma once

#pragma push_macro("GetCurrentTime")
#undef GetCurrentTime

#include "MainWindow.g.h"

#pragma pop_macro("GetCurrentTime")

namespace winrt::NewtonMethod_GUI::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

        int32_t MyProperty();
        void MyProperty(int32_t value);
        
        void OnGoButtonPress(const winrt::Windows::Foundation::IInspectable& sender, const winrt::Microsoft::UI::Xaml::RoutedEventArgs& args);
        void OnResetButtonPress(const winrt::Windows::Foundation::IInspectable& sender, const winrt::Microsoft::UI::Xaml::RoutedEventArgs & args);
    };
}

namespace winrt::NewtonMethod_GUI::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
