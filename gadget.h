#ifndef GADGET_H
#define GADGET_H
#include "item.h"

class Gadget : public Item {
public:
	virtual ~Gadget() {}
	
	Gadget();
	Gadget(string name, string descriptions);
	
	string getType();
	
	void setType(string type);
protected:
	string type;
};
#endif
