/*
Отличие от оригинала в том, что вместо разных изображений для разных
состояний кнопки используется плавное изменение цвета. По этой причине
текстура кнопки должна быть в серых тонах.

Эту анимацию можно сделать без создания отдельного класса:
https://github.com/urho3d/Urho3D/issues/1453.
Но при этом будут заняты события, что не очень удобно, так как
они могут потребоваться для других целей (в Urho3D новый обработчик
события перезаписывает старый).
*/

#pragma once
#include <Urho3D/Urho3DAll.h>

namespace Urho3D
{

class URHO3D_API MyButton : public Button
{
    URHO3D_OBJECT(MyButton, Button);

protected:
    Color pressedColor_;
    Color hoverColor_;
    Color normalColor_;

    // Актуальное состояние IsHovering и IsPressed гарантированно доступно
    // только в функции Update (см https://github.com/urho3d/Urho3D/issues/1453).
    // Поэтому вводим переменные для детектирования смены состояния
    bool oldHover_;
    bool oldPressed_;

public:
    static void RegisterObject(Context* context);

    explicit MyButton(Context* context);

    void Update(float timeStep) override;
    void GetBatches(Vector<UIBatch>& batches, Vector<float>& vertexData, const IntRect& currentScissor) override;

    void SetPressedColor(const Color& color);
    const Color& GetPressedColor() const { return pressedColor_; }

    void SetHoverColor(const Color& color);
    const Color& GetHoverColor() const { return hoverColor_; }

    void SetNormalColor(const Color& color);
    const Color& GetNormalColor() const { return normalColor_; }
};

} // namespace Urho3D
