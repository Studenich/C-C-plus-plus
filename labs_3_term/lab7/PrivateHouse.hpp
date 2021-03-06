#pragma once
#include "House.hpp"
class PrivateHouse :
	public virtual House
{
public:
	PrivateHouse();
	PrivateHouse(int number, std::string street, double yardSquare);
	~PrivateHouse();
	virtual void Show() override;
protected:
	double yardSquare;
};

