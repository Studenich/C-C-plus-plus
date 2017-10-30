///////////////////////////////////////////////////////////
//  Publisher.cpp
//  Implementation of the Class Publisher
//  Created on:      16-���.-2017 18:32:49
//  Original author: ������
///////////////////////////////////////////////////////////

#include "Publisher.h"




Publisher::Publisher(std::string name, std::string organization) : name(name), organization(organization)
{
}

Publisher::Publisher()
{
}

Publisher::~Publisher(){

}





void Publisher::Show(){
	std::cout << "Name of publisher: " << this->name << std::endl
		<< "Name of publisher organization: " << this->organization << std::endl;
}