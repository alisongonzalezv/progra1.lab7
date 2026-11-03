#ifndef ARCHIVOCONTACTOS_H
#define ARCHIVOCONTACTOS_H

#include <QString>
#include <QVector>

struct Contacto {
    QString nombre;
    QString telefono;
    QString correo;
};

class ArchivoContactos {
public:
    QString ruta() const;
    bool guardar(const QString& nombre, const QString& telefono, const QString& correo) const;
    QVector<Contacto> obtenerTodos() const;
    bool reescribirTodo(const QVector<Contacto>& contactos) const;
};

#endif // ARCHIVOCONTACTOS_H