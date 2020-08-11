#include "pch.h"
#include "MainWindow.h"
#include "MainWindow.g.cpp"

import Newton;

//using namespace winrt;
//using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::NewtonMethod_GUI::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();
    }

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void MainWindow::OnGoButtonPress(const winrt::Windows::Foundation::IInspectable&, const winrt::Microsoft::UI::Xaml::RoutedEventArgs&)
    {
        // Check starting value (floating point number)
        const wchar_t* wszStart = this->startingValue().Text().c_str();
        char szStart[128] = { 0 };
        BOOL bUsedDefaultChar;
        char defaultChar = ' ';
        WideCharToMultiByte(
            CP_ACP,
            0,
            wszStart,
            -1,
            szStart,
            sizeof(szStart),
            &defaultChar,
            &bUsedDefaultChar);
        
        double start;
        std::errc ec = (std::from_chars(szStart, szStart + std::strlen(szStart), start)).ec;

        if (ec == std::errc{})
        {
            wchar_t wszTmp[64] = { 0 };
            std::swprintf(wszTmp, 64, L"%lg", start);
            this->startingValue().Text(wszTmp);
            this->invalidMessage().Text(L"");
        }
        else
        {
            this->invalidMessage().Text(L"Please check your input.");
            return;
        }

        // Check coefficient input
        const std::wstring coeffInput(this->coefficients().Text().c_str());
        std::wstringstream wss;
        double coeff;
        std::vector<double> vCoefficients;
        
        wss << coeffInput;
        while (wss >> coeff)
            vCoefficients.push_back(coeff);
        
        // Check if input was parsed to the end
        if (!wss.eof())
        {
            this->invalidMessage().Text(L"Please check your input.");
            return;
        }
        
        // Show polynomial
        const Polynomial f(vCoefficients);
        this->polynomial().Text(f.ToString());

        // Initiate Newton Method
        const NewtonMethodResult result = NewtonMethod(f, start);
        if (result.success)
        {
            wchar_t wszTmp[64] = { 0 };
            std::swprintf(wszTmp, 64, L"%lg", result.answer);
            this->answer().Text(wszTmp);
        }
        else
        {
            this->answer().Text(result.reason);
        }
    }

    void MainWindow::OnResetButtonPress(const winrt::Windows::Foundation::IInspectable&, const winrt::Microsoft::UI::Xaml::RoutedEventArgs&)
    {
        this->coefficients().Text(L"");
        this->startingValue().Text(L"");
        this->invalidMessage().Text(L"");
        this->polynomial().Text(L"");
        this->answer().Text(L"");
    }
}

