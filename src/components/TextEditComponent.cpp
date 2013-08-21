#include "TextEditComponent.h"
#include "../Log.h"
#include "../Font.h"
#include "../Window.h"
#include "../Renderer.h"
#include "ComponentListComponent.h"

TextEditComponent::TextEditComponent(Window* window) : GuiComponent(window),
	mBox(window, 0, 0, 0, 0), mFocused(false), mAllowResize(true)
{
	addChild(&mBox);
	
	onFocusLost();

	setSize(256, (float)getFont()->getHeight());
}

void TextEditComponent::onFocusGained()
{
	mBox.setHorizontalImage(":/glow_hor.png");
	mBox.setVerticalImage(":/glow_vert.png");
	mBox.setBorderColor(0x51CCFF00 | getOpacity());

	SDL_StartTextInput();
	mFocused = true;
}

void TextEditComponent::onFocusLost()
{
	mBox.setHorizontalImage(":/glow_off_hor.png");
	mBox.setVerticalImage(":/glow_off_vert.png");
	mBox.setBorderColor(0xFFFFFF00 | getOpacity());

	SDL_StopTextInput();
	mFocused = false;
}

void TextEditComponent::onSizeChanged()
{
	mBox.setSize(mSize);
}

void TextEditComponent::setValue(const std::string& val)
{
	mText = val;
	onTextChanged();
}

std::string TextEditComponent::getValue() const
{
	return mText;
}

void TextEditComponent::textInput(const char* text)
{
	if(mFocused)
	{
		if(text[0] == '\b')
		{
			if(mText.length() > 0)
				mText.erase(mText.end() - 1, mText.end());
		}else{
			mText += text;
		}
	}

	onTextChanged();
}

void TextEditComponent::onTextChanged()
{
	if(mAllowResize)
	{
		float y = getFont()->sizeWrappedText(mText, mSize.x()).y();
		if(y == 0)
			y = getFont()->getHeight();
		
		setSize(mSize.x(), y);
	}
	
	ComponentListComponent* cmp = dynamic_cast<ComponentListComponent*>(getParent());
	if(cmp)
		cmp->updateComponent(this);
}

void TextEditComponent::setAllowResize(bool allow)
{
	mAllowResize = allow;
	onTextChanged();
}

void TextEditComponent::render(const Eigen::Affine3f& parentTrans)
{
	Eigen::Affine3f trans = getTransform() * parentTrans;
	renderChildren(trans);

	Renderer::setMatrix(trans);
	
	std::shared_ptr<Font> f = getFont();
	f->drawWrappedText(mText, Eigen::Vector2f(0, 0), mSize.x(), 0x000000 | getOpacity());
}

std::shared_ptr<Font> TextEditComponent::getFont()
{
	return Font::get(*mWindow->getResourceManager(), Font::getDefaultPath(), FONT_SIZE_SMALL);
}
