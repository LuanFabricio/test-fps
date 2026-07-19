#include "general.h"
#include <assert.h>

#include "system/attributes.h"

void attributes_add(attributes_t* att1, const attributes_t att2)
{
	att1->max_health 		+= att2.max_health;
	att1->armor 			+= att2.armor;
	att1->damage 		  	+= att2.damage;
	att1->elemental_multiplier 	+= att2.elemental_multiplier;
}

attributes_t attributes_gen_random(const float min_value, const float max_value)
{
	assert(max_value > min_value && "`max` should be greater than `min`");

	const float min_max_offset = max_value - min_value;
	attributes_t attribute =  {
		.max_health = min_value + randf() * min_max_offset,
		.elemental_multiplier = min_value + randf() * min_max_offset,
		.armor = min_value + randf() * min_max_offset,
		.damage = min_value + randf() * min_max_offset,
	};
	const float min_current_health_pc = 0.05;
	float current_health_pc = randf();
	if (current_health_pc < min_current_health_pc) {
		current_health_pc = min_current_health_pc;
	}
	attribute.current_health = attribute.max_health * current_health_pc;

	return attribute;
}
