#include "Dragger.h"

Dragger::Dragger(const Vector& newStartMousePosition, const Vector& newStartThingPosition)
	: isCurrentlyDragging(true), startMousePosition(newStartMousePosition), startThingPosition(newStartThingPosition)
{

}