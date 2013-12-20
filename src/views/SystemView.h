#pragma once

#include "../GuiComponent.h"
#include "../components/ImageComponent.h"
#include "../components/TextComponent.h"
#include "../components/ScrollableContainer.h"

class SystemData;

class SystemView : public GuiComponent
{
public:
	SystemView(Window* window, SystemData* system);

	void updateData();

private:
	SystemData* mSystem;

	TextComponent mHeaderText;
	ImageComponent mHeaderImage;
	ImageComponent mImage;
};