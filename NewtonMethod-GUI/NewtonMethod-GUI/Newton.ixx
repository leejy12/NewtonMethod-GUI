module;

#include "pch.h"
#include <fstream>

export module Newton;

export
constexpr double EPSILON = 0.000000001;

export
struct NewtonMethodResult
{
    bool success;
    double answer;
    winrt::hstring reason;
};

export
class Polynomial
{
private:
    std::size_t _degree;
    std::vector<double> _vCoeffecients;

public:
    Polynomial(const std::vector<double>& cff)
        : _degree(cff.size() - 1), _vCoeffecients(cff)
    {}

    double operator()(const double x) const
    {
        double res = 0;
        for (int i = 0; i <= _degree; i++)
            res += _vCoeffecients[i] * std::pow(x, _degree - i);
        return res;
    }

    Polynomial Derivative() const
    {
        // Handle constant functions. GH-#5
        if (_degree == 0)
            return Polynomial({ 0 });

        std::vector<double> vDerivedCoefficients(_degree);

        // Power rule
        for (int i = 0; i < _degree; i++)
            vDerivedCoefficients[i] = (_degree - i) * _vCoeffecients[i];

        return Polynomial(vDerivedCoefficients);
    }

    winrt::hstring ToString() const
    {
        if (IsZeroFunction())
            return winrt::hstring(L"0");

        std::wstringstream wss;

        // We probably shouldn't be doing == and != operations on floating point values.
        for (int i = 0; i < _degree; i++)
        {
            if (_vCoeffecients[i] == 0)
            {
                if (_vCoeffecients[i + 1] > 0)
                    wss << L'+';
                continue;
            }
            if (_vCoeffecients[i] == 1.0)
                wss << L'x';
            else if (_vCoeffecients[i] == -1)
                wss << L"-x";
            else
                wss << _vCoeffecients[i] << L'x';
            if (i != _degree - 1)
                wss << L'^' << _degree - i;
            if (_vCoeffecients[i + 1] > 0)
                wss << L'+';
        }
        if (_vCoeffecients[_degree] != 0)
            wss << _vCoeffecients[_degree];
        
        return winrt::hstring(wss.str().c_str());
    }

    bool IsZeroFunction() const
    {
        return (_degree == 0) && (_vCoeffecients[0] < EPSILON);
    }
};

export
NewtonMethodResult NewtonMethod(const Polynomial& f, const double ix)
{
    // Check if f is a zero function.    
    if (f.IsZeroFunction())
        return { false, NAN, L"Zero Function" };

    // initial guess is very close to root
    if (std::abs(f(ix)) < EPSILON)
        return { true, ix, L"" };

    double x0 = ix, x1, y, yprime;
    const Polynomial fprime = f.Derivative();
    
    static constexpr int MAX_ITERATION = 30;
    for (int i = 0; i < MAX_ITERATION; i++)
    {
        y = f(x0);
        yprime = fprime(x0);

        // Check for division by zero.
        if (std::abs(yprime) < EPSILON)
            return { false, NAN, L"Division By Zero" };
        x1 = x0 - y / yprime;

        // If change from recursive step is minimal (i.e. close enough to root), return
        if (std::abs(x1 - x0) < EPSILON)
            return { true, x1, L"" };
        else
            x0 = x1;
    }

    return { false, NAN, L"Did Not Converge" };
}

