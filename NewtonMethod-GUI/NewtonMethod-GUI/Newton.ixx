module;

#include "pch.h"
#include <fstream>

export module Newton;

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
	std::vector<double> _coeff;

public:
	Polynomial(const std::vector<double>& cff)
		: _degree(cff.size() - 1), _coeff(cff)
	{}

	double operator()(const double x) const
	{
		double res = 0;
		for (int i = 0; i <= _degree; i++)
			res += _coeff[i] * std::pow(x, _degree - i);
		return res;
	}

	Polynomial Derivative() const
	{
		std::vector<double> dvCoeff(_degree);

		// Power rule
		for (int i = 0; i < _degree; i++)
			dvCoeff[i] = (_degree - i) * _coeff[i];

		return Polynomial(dvCoeff);
	}

	winrt::hstring ToString() const
	{
		std::wstringstream wss;
		for (int i = 0; i < _degree; i++)
		{
			if (_coeff[i] == 0)
			{
				if (_coeff[i + 1] > 0)
					wss << L'+';
				continue;
			}
			if (_coeff[i] == 1.0)
				wss << L'x';
			else if (_coeff[i] == -1)
				wss << L"-x";
			else
				wss << _coeff[i] << L'x';
			if (i != _degree - 1)
				wss << L'^' << _degree - i;
			if (_coeff[i + 1] > 0)
				wss << L'+';
		}
		if (_coeff[_degree] != 0)
			wss << _coeff[_degree];
		
		return winrt::hstring(wss.str().c_str());
	}
};

export
NewtonMethodResult NewtonMethod(const Polynomial& f, double ix)
{
	static constexpr double EPSILON = 0.000000001;
	NewtonMethodResult res;

	// initial guess is very close to root
	if (f(ix) < EPSILON)
		return { true, ix, L"" };

	double x0 = ix, x1, y, yprime;
	Polynomial fprime = f.Derivative();
	
	// std::ofstream log("");

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

