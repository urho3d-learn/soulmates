/*
Основано на Button.cpp.
*/

#include "MyButton.h"

const char* UI_CATEGORY = "UI";

// Цвета по умолчанию.
#define BUTTON_NORMAL_COLOR Color(0.5f, 0.0f, 0.0f)
#define BUTTON_HOVER_COLOR Color::RED
#define BUTTON_PRESSED_COLOR Color::YELLOW

namespace Urho3D
{

MyButton::MyButton(Context* context) :
    Button(context),
    pressedColor_(BUTTON_PRESSED_COLOR),
    hoverColor_(BUTTON_HOVER_COLOR),
    normalColor_(BUTTON_NORMAL_COLOR),
    oldHover_(false),
    oldPressed_(false)
{
    //SetEnabled(true);
    //focusMode_ = FM_FOCUSABLE;

    SetColor(normalColor_);
}

void MyButton::SetNormalColor(const Color& color)
{
    normalColor_ = color;

    // Мгновенная смена цвета. Иначе при загрузке цвет кнопки будет дефолтным тёмно-красным,
    // и только потом плавно изменится до заданного в xml-файле значения "Normal Color".
    if (!pressed_ && !hovering_)
    {
        SetColor(normalColor_);
        RemoveAttributeAnimation("Color");
    }
}

void MyButton::SetPressedColor(const Color& color)
{
    pressedColor_ = color;

    if (pressed_)
    {
        SetColor(pressedColor_);
        RemoveAttributeAnimation("Color");
    }
}

void MyButton::SetHoverColor(const Color& color)
{
    hoverColor_ = color;

    if (!pressed_ && hovering_)
    {
        SetColor(hoverColor_);
        RemoveAttributeAnimation("Color");
    }
}

void MyButton::RegisterObject(Context* context)
{
    context->RegisterFactory<MyButton>(UI_CATEGORY);

    URHO3D_COPY_BASE_ATTRIBUTES(Button);

    URHO3D_ACCESSOR_ATTRIBUTE("Hover Color", GetHoverColor, SetHoverColor, BUTTON_HOVER_COLOR, AM_FILE);
    URHO3D_ACCESSOR_ATTRIBUTE("Pressed Color", GetPressedColor, SetPressedColor, BUTTON_PRESSED_COLOR, AM_FILE);
    URHO3D_ACCESSOR_ATTRIBUTE("Normal Color", GetNormalColor, SetNormalColor, BUTTON_NORMAL_COLOR, AM_FILE);
}

void MyButton::GetBatches(Vector<UIBatch>& batches, Vector<float>& vertexData, const IntRect& currentScissor)
{
    // Смещение всегда нулевое, свойство hoverOffset базового класса игнорируется.
    BorderImage::GetBatches(batches, vertexData, currentScissor, IntVector2::ZERO);
}

void MyButton::Update(float timeStep)
{
    if (!hovering_ && pressed_)
        SetPressed(false);

    // Зафиксирована смена состояния, поэтому запускаем анимацию цвета.
    if (oldPressed_ != pressed_ || oldHover_ != hovering_)
    {
        oldPressed_ = pressed_;
        oldHover_ = hovering_;

        Color currentColor = GetColor(Corner::C_TOPLEFT);
        Color targetColor = currentColor;

        if (pressed_)
            targetColor = pressedColor_;
        else if (hovering_)
            targetColor = hoverColor_;
        else
            targetColor = normalColor_;

        if (currentColor != targetColor)
        {
            SharedPtr<ValueAnimation> colorAnimation(new ValueAnimation(context_));
            colorAnimation->SetKeyFrame(0.0f, currentColor);
            colorAnimation->SetKeyFrame(0.2f, targetColor);
            SetAttributeAnimation("Color", colorAnimation, WM_ONCE);
        }
        else
        {
            // Теоретически возможно, что текущий и целевой цвет уже совпадают,
            // но запущена анимация в сторону какого-то другого цвета.
            RemoveAttributeAnimation("Color");
        }
    }

    if (pressed_ && repeatRate_ > 0.0f)
    {
        repeatTimer_ -= timeStep;
        if (repeatTimer_ <= 0.0f)
        {
            repeatTimer_ += 1.0f / repeatRate_;

            using namespace Pressed;

            VariantMap& eventData = GetEventDataMap();
            eventData[P_ELEMENT] = this;
            SendEvent(E_PRESSED, eventData);
        }
    }
}

} // namespace Urho3D
