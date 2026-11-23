#include "textfield.h"

TextFieldBase::TextFieldBase(float w, float h)
    : Widget(w, h) {}


void TextFieldBase::set_editable(bool status) {
    editable = status;
}

void TextFieldBase::on_click(float /*mx*/, float /*my*/) {
    if (editable) focused = true;
}