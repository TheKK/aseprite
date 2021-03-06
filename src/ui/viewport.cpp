// Aseprite UI Library
// Copyright (C) 2001-2013, 2015  David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gfx/point.h"
#include "gfx/size.h"
#include "ui/message.h"
#include "ui/preferred_size_event.h"
#include "ui/resize_event.h"
#include "ui/theme.h"
#include "ui/view.h"
#include "ui/viewport.h"

namespace ui {

using namespace gfx;

Viewport::Viewport()
  : Widget(kViewViewportWidget)
{
  initTheme();
}

void Viewport::onResize(ResizeEvent& ev)
{
  Rect rect = ev.getBounds();
  setBoundsQuietly(rect);

  Point scroll = static_cast<View*>(this->getParent())->getViewScroll();

  Rect cpos(0, 0, 0, 0);
  cpos.x = rect.x + border().left() - scroll.x;
  cpos.y = rect.y + border().top() - scroll.y;

  UI_FOREACH_WIDGET(getChildren(), it) {
    Widget* child = *it;
    Size reqSize = child->getPreferredSize();

    cpos.w = MAX(reqSize.w, rect.w - border().width());
    cpos.h = MAX(reqSize.h, rect.h - border().height());

    child->setBounds(cpos);
  }
}

void Viewport::onPreferredSize(PreferredSizeEvent& ev)
{
  ev.setPreferredSize(gfx::Size(1 + border().width(),
                                1 + border().height()));
}

void Viewport::onPaint(PaintEvent& ev)
{
  getTheme()->paintViewViewport(ev);
}

Size Viewport::calculateNeededSize()
{
  Size maxSize(0, 0);
  Size reqSize;

  UI_FOREACH_WIDGET(getChildren(), it) {
    reqSize = (*it)->getPreferredSize();
    maxSize.w = MAX(maxSize.w, reqSize.w);
    maxSize.h = MAX(maxSize.h, reqSize.h);
  }

  return maxSize;
}

} // namespace ui
