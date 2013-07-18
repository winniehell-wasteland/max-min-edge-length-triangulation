#include "svg_painter.h"

namespace MMLT
{

const int SVGPainter::SVG_PADDING = 10;
const double SVGPainter::SVG_SCALE = 4.0;

SVGPainter::SVGPainter(const QString& file_prefix, const QString& file_name, const BoundingBox& bbox) :
    QPainter(),
    generator_(),
    pen_()
{
    generator_.setFileName(file_prefix + "_" + file_name);
    generator_.setTitle(file_prefix);

    generator_.setSize(
                QSize(
                    CGAL::to_double(bbox.width()) + 2 * SVG_PADDING,
                    CGAL::to_double(bbox.height()) + 2 * SVG_PADDING
                    ) * SVG_SCALE
                );

    generator_.setViewBox(
                QRect(
                    CGAL::to_double(SVG_SCALE * (bbox.xmin() - SVG_PADDING)),
                    CGAL::to_double(SVG_SCALE * (bbox.ymin() - SVG_PADDING)),
                    bbox.width(),
                    bbox.height()
                    )
                );

    this->begin(&generator_);

    pen_.setWidthF(0.5);
    pen_.setCosmetic(true);
    this->setPen(pen_);

    this->scale(SVG_SCALE, SVG_SCALE);
}

void SVGPainter::setPenColor(const QColor& color)
{
    pen_.setColor(color);
    this->setPen(pen_);
}

void SVGPainter::setPenWidth(int width)
{
    pen_.setWidth(width);
    this->setPen(pen_);
}

SVGPainter::~SVGPainter()
{
    this->end();
}

}
