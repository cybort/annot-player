// graphicshaloeffect.cc
// 5/3/2012
// See: gui/effects/qgraphicseffect.cpp
#include "module/graphicseffect/graphicshaloeffect.h"
#include "module/graphicseffect/graphicshaloeffect_p.h"

#include <QtGui/qgraphicsitem.h>

#include <QtGui/qimage.h>
#include <QtGui/qpainter.h>
#include <QtGui/qpaintengine.h>
#include <QtCore/qrect.h>
#include <QtCore/qdebug.h>
#include <private/qdrawhelper_p.h>

// - Construction -

GraphicsHaloEffect::GraphicsHaloEffect(QObject *parent)
  : Base(*new GraphicsHaloEffectPrivate, parent)
{ }

// - Properties -

QPointF
GraphicsHaloEffect::offset() const
{
  Q_D(const GraphicsHaloEffect);
  return d->filter->offset();
}

void
GraphicsHaloEffect::setOffset(const QPointF &offset)
{
  Q_D(GraphicsHaloEffect);
  if (d->filter->offset() == offset)
    return;

  d->filter->setOffset(offset);
  updateBoundingRect();
  emit offsetChanged(offset);
}

qreal
GraphicsHaloEffect::blurRadius() const
{
  Q_D(const GraphicsHaloEffect);
  return d->filter->blurRadius();
}

void
GraphicsHaloEffect::setBlurRadius(qreal blurRadius)
{
  Q_D(GraphicsHaloEffect);
  if (qFuzzyCompare(d->filter->blurRadius(), blurRadius))
    return;

  d->filter->setBlurRadius(blurRadius);
  updateBoundingRect();
  emit blurRadiusChanged(blurRadius);
}

QColor
GraphicsHaloEffect::color() const
{
  Q_D(const GraphicsHaloEffect);
  return d->filter->color();
}

void
GraphicsHaloEffect::setColor(const QColor &color)
{
  Q_D(GraphicsHaloEffect);
  if (d->filter->color() == color)
    return;

  d->filter->setColor(color);
  update();
  //emit colorChanged(color);
}

qreal
GraphicsHaloEffect::opacity() const
{
  Q_D(const GraphicsHaloEffect);
  return d->opacity;
}

void
GraphicsHaloEffect::setOpacity(qreal opacity)
{
  Q_D(GraphicsHaloEffect);
  if (qFuzzyCompare(d->opacity, opacity))
    return;

  d->opacity = opacity;
  update();
  //emit opacityChanged(opacity);
}

QRectF
GraphicsHaloEffect::boundingRectFor(const QRectF &rect) const
{
  Q_D(const GraphicsHaloEffect);
  return d->filter->boundingRectFor(rect);
}

// - Paint -

void
GraphicsHaloEffect::draw(QPainter *painter)
{
  Q_D(GraphicsHaloEffect);
  if (!qFuzzyCompare(d->opacity, 1))
    painter->setOpacity(d->opacity);
  if (d->filter->blurRadius() <= 0 && d->filter->offset().isNull()) {
    drawSource(painter);
    return;
  }

  PixmapPadMode mode = PadToEffectiveBoundingRect;
  if (painter->paintEngine()->type() == QPaintEngine::OpenGL2)
    mode = NoPad;

  // Draw pixmap in device coordinates to avoid pixmap scaling.
  QPoint offset;
  const QPixmap pixmap = sourcePixmap(Qt::DeviceCoordinates, &offset, mode);
  if (pixmap.isNull())
    return;

  QTransform restoreTransform = painter->worldTransform();
  painter->setWorldTransform(QTransform());
  d->filter->draw(painter, offset, pixmap);
  painter->setWorldTransform(restoreTransform);
}

// EOF
/*

void
 QGraphicsOpacityEffect::draw(QPainter *painter)
{
  Q_D(QGraphicsOpacityEffect);

  // Transparent; nothing to draw.
  if (d->isFullyTransparent)
    return;

  // Opaque; draw directly without going through a pixmap.
  if (d->isFullyOpaque && !d->hasOpacityMask) {
    drawSource(painter);
    return;
  }

  QPoint offset;
  Qt::CoordinateSystem system = sourceIsPixmap() ? Qt::LogicalCoordinates : Qt::DeviceCoordinates;
  QPixmap pixmap = sourcePixmap(system, &offset, QGraphicsEffect::NoPad);
  if (pixmap.isNull())
    return;

  painter->save();
  painter->setOpacity(d->opacity);

  if (d->hasOpacityMask) {
    QPainter pixmapPainter(&pixmap);
    pixmapPainter.setRenderHints(painter->renderHints());
    pixmapPainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    if (system == Qt::DeviceCoordinates) {
      QTransform worldTransform = painter->worldTransform();
      worldTransform *= QTransform::fromTranslate(-offset.x(), -offset.y());
      pixmapPainter.setWorldTransform(worldTransform);
      pixmapPainter.fillRect(sourceBoundingRect(), d->opacityMask);
    } else {
      pixmapPainter.translate(-offset);
      pixmapPainter.fillRect(pixmap.rect(), d->opacityMask);
    }
  }

  if (system == Qt::DeviceCoordinates)
    painter->setWorldTransform(QTransform());

  painter->drawPixmap(offset, pixmap);
  painter->restore();
}
*/