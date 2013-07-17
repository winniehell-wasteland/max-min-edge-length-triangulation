#pragma once
#ifndef MMLT_UTILS_SVG_PAINTER_H
#define MMLT_UTILS_SVG_PAINTER_H

#include "config.h"

#include <QPainter>
#include <QSvgGenerator>

#include "cgal/bounding_box.h"

class SVGPainter :
        public QPainter
{
public:
    SVGPainter(const QString& file_prefix, const QString& file_name, const BoundingBox& bbox);

    void setPenColor(const QColor& color);

    void setPenWidth(int width);

    ~SVGPainter();

private:
    /**
     * padding for SVG images
    */
    static const int SVG_PADDING;

    /**
     * scale for SVG images
    */
    static const double SVG_SCALE;

    QSvgGenerator generator_;
    QPen          pen_;
};

#endif // MMLT_UTILS_SVG_PAINTER_H
