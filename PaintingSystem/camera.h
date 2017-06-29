#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>

class Camera
{
public:
    Camera( void );
    void SetPos( const QVector3D pos, bool _try = false );
    inline QVector3D& Pos( void ) { return m_Pos; }
    inline QVector3D& Len( void ) { return m_Len; }
    inline qreal& RotateY( void ) { return m_RotateY; }
    inline qreal& RotateH( void ) { return m_RotateH; }
    void SetRotateY( qreal rotateY, bool _try = false );
    void SetRotateH( qreal rotateH, bool _try = false );
    void ZoomIn( qreal step );

    void Apply( void );
private:
    QVector3D           m_Pos;
    QVector3D           m_Len;
    qreal               m_RotateY, m_RotateH;

    // 尝试的部分
    QVector3D           m_TryPos;
    qreal               m_TryRotateY, m_TryRotateH;
    bool                m_Try;
};

#endif // CAMERA_H
