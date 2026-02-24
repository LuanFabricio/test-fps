#include "system/attributes.h"

void attributes_add(attributes_t* att1, const attributes_t att2)
{
	att1->max_health 		+= att2.max_health;
	att1->armor 			+= att2.armor;
	att1->damage 		  	+= att2.damage;
	att1->elemental_multiplier 	+= att2.elemental_multiplier;
}
