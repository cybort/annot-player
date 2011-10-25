// trendview.cc
// 7/15/2011

#include "trendview.h"
#include "global.h"
#include "uistyle.h"
#include <QtGui>
#include <QtWebKit>

// - Constructions -

#define WINDOW_FLAGS ( \
  Qt::Dialog | \
  Qt::CustomizeWindowHint | \
  Qt::WindowTitleHint | \
  Qt::WindowSystemMenuHint | \
  Qt::WindowMinMaxButtonsHint | \
  Qt::WindowCloseButtonHint )

TrendView::TrendView(QWidget *parent)
  : Base(parent)
{
  setWindowFlags(WINDOW_FLAGS);
  setContentsMargins(0, 0, 0, 0);
  setHomePage(G_STARTPAGE_URL);

  const bool useUiStyleQss = false;
  UiStyle::globalInstance()->setWindowStyle(this, useUiStyleQss);
}

// - Events -

void
TrendView::setVisible(bool visible)
{
  Base::setVisible(visible);
  if (visible && tabCount() == 0)
    newTab();
}

// EOF